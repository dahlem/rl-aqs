// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a spevectoral exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file ConjugateGradient.hh
 * Declaration/Implementation of the backpropagation neuro network training algorithm.
 */
#ifndef __CONJUGATEGRADIENT_HH__
#define __CONJUGATEGRADIENT_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <algorithm>
#include <cstring>
#include <cmath>
#include <limits>
#include <vector>

#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>

#include "Vector.hh"
namespace dutil = des::utils;


typedef boost::shared_array <double> DoubleSA;
typedef boost::shared_array <DoubleSA> DoubleSM;


namespace des
{
namespace nnet
{

#define ERRTOL          1e-6
#define TRAINING_RATE   1.0
#define GOLD            1.618034
#define GLIMIT          100.0
#define TINY            1.0e-20
#define ITMAX           500
#define CGOLD           0.3819660


template<class T>
inline T SIGN(const T &a, const T &b)
{
    return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);
}



template <class NeuralNetwork,
          class Objective>
class ConjugateGradient
{
public:
    ConjugateGradient(NeuralNetwork p_nnet, Objective p_objective)
        : m_nnet(p_nnet), m_objective(p_objective), m_errtol(ERRTOL),
          m_optimalTrainingRate(TRAINING_RATE), m_initialTrainingRate(TRAINING_RATE),
          m_isFirst(true), m_iterMax(ITMAX)
        {
            init();
        }

    ConjugateGradient(NeuralNetwork p_nnet, Objective p_objective, double p_trainingRate,
                      double p_errtol, boost::uint16_t p_iterMax)
        : m_nnet(p_nnet), m_objective(p_objective), m_errtol(p_errtol),
          m_optimalTrainingRate(p_trainingRate), m_initialTrainingRate(p_trainingRate),
          m_isFirst(true), m_iterMax(p_iterMax)
        {
            init();
        }

    ~ConjugateGradient()
        {}


    void train(DoubleSA p_targets)
        {
            double error = m_objective->error(
                p_targets, m_nnet->getOutputNeurons(), m_nnet->getNumOutputs());

            // don't start the training if the error is small enough
            if (error < m_errtol) {
                return;
            }

            m_objective->calc_gradient(p_targets);
            DoubleSA gradient = m_objective->getGradient();

            // if first training init training direction
            // otherwise use the Polak Ribiere
            if (m_isFirst) {
                for (boost::uint16_t i = 0; i < m_nnet->getNumFree(); ++i) {
                    m_trainingDirection[i] = -gradient[i];
                }
            } else {
                DoubleSA trainingDir =
                    getTrainingDirection(m_gradientOld, gradient, m_trainingDirection);
                m_trainingDirection = trainingDir;
            }

            double slope = dutil::Vector::dotproduct(
                m_nnet->getNumFree(), gradient, m_trainingDirection);

            // reset the training direction
            if (slope >= 0.0) {
                for (boost::uint16_t i = 0; i < m_nnet->getNumFree(); ++i) {
                    m_trainingDirection[i] = -gradient[i];
                }
            }

            DoubleSA weights = m_nnet->getWeights();

            m_optimalTrainingRate = getOptimalTrainingRate(
                p_targets, m_optimalTrainingRate, error, weights, m_trainingDirection);

            // double - double < tol
            if (m_optimalTrainingRate == 0.0) {
                return;
            }

            updateWeights();

            m_gradientOld = gradient;

            m_isFirst = false;
        }


private:
    ConjugateGradient()
        {}

    void init()
        {
            m_numFree = m_nnet->getNumFree();

            m_trainingDirection = DoubleSA(new double[m_nnet->getNumFree()]);
            memset(m_trainingDirection.get(), 0, m_numFree * sizeof(double));

            m_gradientOld = DoubleSA(new double[m_nnet->getNumFree()]);
            memset(m_gradientOld.get(), 0, m_numFree * sizeof(double));

            m_optimalTrainingRateTol = 1.0e-3;
        }

    void updateWeights()
        {
            boost::uint16_t index = 0;

            for (boost::uint16_t j = 0; j < m_nnet->getNumHidden(); ++j) {
                for (boost::uint16_t i = 0; i <= m_nnet->getNumInputs(); ++i) {
                    m_nnet->addWeightInputHidden(
                        i, j, m_optimalTrainingRate * m_trainingDirection[index]);
                    index++;
                }
            }

            for (boost::uint16_t k = 0; k < m_nnet->getNumOutputs(); ++k) {
                for (boost::uint16_t j = 0; j <= m_nnet->getNumHidden(); ++j) {
                    m_nnet->addWeightHiddenOutput(
                        j, k, m_optimalTrainingRate * m_trainingDirection[index]);
                    index++;
                }
            }
        }

    DoubleSA getTrainingDirection(DoubleSA p_gradientOld, DoubleSA p_gradient,
                                  DoubleSA p_trainingDirection)
        {
            DoubleSA trainingDirection = DoubleSA(new double[m_nnet->getNumFree()]);
            double pr_param = getPolakRibiereParameter(p_gradientOld, p_gradient);

            for (boost::uint16_t i = 0; i < m_numFree; ++i) {
                trainingDirection[i] = -p_gradient[i] + p_trainingDirection[i] * pr_param;
            }

            return trainingDirection;
        }

    double getPolakRibiereParameter(DoubleSA p_gradientOld, DoubleSA p_gradient)
        {
            double parameter, numerator, denominator;

            parameter = numerator = denominator = 0.0;

            for (boost::uint16_t i = 0; i < m_numFree; ++i) {
                denominator += p_gradientOld[i] * p_gradientOld[i];
                numerator += (p_gradient[i] - p_gradientOld[i]) * p_gradient[i];
            }

            if (denominator != 0.0) {
                parameter = numerator / denominator;
            }

            if (parameter < 0.0) {
                parameter = 0.0;
            }

            if (parameter > 1.0) {
                parameter = 1.0;
            }

            return parameter;
        }

	inline void shft2(double &a, double &b, const double c)
        {
            a = b;
            b = c;
        }

	inline void shft3(double &a, double &b, double &c, const double d)
        {
            a = b;
            b = c;
            c = d;
        }

	inline void mov3(double &a, double &b, double &c, const double d, const double e,
                     const double f)
        {
            a = d; b = e; c = f;
        }

    double getOptimalTrainingRate(DoubleSA p_targets, double p_trainingRate, double p_error,
                                  DoubleSA p_weights, DoubleSA p_trainingDirection)
        {
            double ax, bx, cx, fa, fb, fc, fu;

            ax = 0.0;
            bx = p_trainingRate;
            fa = p_error;

            // bracketing
            DoubleSA potentialWeights = DoubleSA(new double[m_numFree]);
            for (boost::uint16_t i = 0; i < m_numFree; ++i) {
                potentialWeights[i] = p_weights[i] + p_trainingDirection[i] * bx;
            }

            fb = m_objective->potentialError(potentialWeights, p_targets);

            if (fb > fa) {
                std::swap(ax, bx);
                std::swap(fb, fa);
            }

            cx = bx + GOLD * (bx - ax);

            for (boost::uint16_t i = 0; i < m_numFree; ++i) {
                potentialWeights[i] = p_weights[i] + p_trainingDirection[i] * cx;
            }

            fc = m_objective->potentialError(potentialWeights, p_targets);

            while (fb > fc) {
                double r = (bx - ax) * (fb - fc);
                double q = (bx - cx) * (fb - fa);
                double u = bx - ((bx - cx) * q - (bx - ax) * r) /
                    (2.0 * SIGN(std::max(static_cast<double> (fabs(q - r)), TINY), q - r));
                double ulim = bx + GLIMIT * (cx - bx);

                if ((bx - u) * (u - cx) > 0.0) {
                    for (boost::uint16_t i = 0; i < m_numFree; ++i) {
                        potentialWeights[i] = p_weights[i] + p_trainingDirection[i] * u;
                    }

                    fu = m_objective->potentialError(potentialWeights, p_targets);

                    if (fu < fc) {
                        ax = bx;
                        bx = u;
                        fa = fb;
                        fb = fu;
                        break;
                    } else if (fu > fb) {
                        cx = u;
                        fc = fu;
                        break;
                    }
                    u = cx + GOLD * (cx - bx);

                    for (boost::uint16_t i = 0; i < m_numFree; ++i) {
                        potentialWeights[i] = p_weights[i] + p_trainingDirection[i] * u;
                    }

                    fu = m_objective->potentialError(potentialWeights, p_targets);
                } else if ((cx-u)*(u-ulim) > 0.0) {
                    for (boost::uint16_t i = 0; i < m_numFree; ++i) {
                        potentialWeights[i] = p_weights[i] + p_trainingDirection[i] * u;
                    }

                    fu = m_objective->potentialError(potentialWeights, p_targets);

                    if (fu < fc) {
                        shft3(bx, cx, u, u + GOLD * (u - cx));
                        for (boost::uint16_t i = 0; i < m_numFree; ++i) {
                            potentialWeights[i] = p_weights[i] + p_trainingDirection[i] * u;
                        }

                        shft3(fb, fc, fu,
                              m_objective->potentialError(potentialWeights, p_targets));
                    }
                } else if ((u - ulim) * (ulim - cx) >= 0.0) {
                    u = ulim;
                    for (boost::uint16_t i = 0; i < m_numFree; ++i) {
                        potentialWeights[i] = p_weights[i] + p_trainingDirection[i] * u;
                    }

                    fu = m_objective->potentialError(potentialWeights, p_targets);
                } else {
                    u = cx + GOLD * (cx - bx);
                    for (boost::uint16_t i = 0; i < m_numFree; ++i) {
                        potentialWeights[i] = p_weights[i] + p_trainingDirection[i] * u;
                    }

                    fu = m_objective->potentialError(potentialWeights, p_targets);
                }

                shft3(ax, bx, cx, u);
                shft3(fa, fb, fc, fu);
            }

            const double ZEPS = std::numeric_limits<double>::epsilon() * 1.0e-3;
            const double tol = 3.0e-8;
            double a, b, d = 0.0, etemp, fv, fw, fx;
            double p, q, r, tol1, tol2, u, v, w, x, xm;
            double e = 0.0;

            a = (ax < cx ? ax : cx);
            b = (ax > cx ? ax : cx);
            x = w = v = bx;

            for (boost::uint16_t i = 0; i < m_numFree; ++i) {
                potentialWeights[i] = p_weights[i] + p_trainingDirection[i] * x;
            }

            fx = m_objective->potentialError(potentialWeights, p_targets);
            fw = fv = fx;

            for (boost::uint16_t iter = 0; iter < ITMAX; ++iter) {
                xm = 0.5 * (a + b);
                tol2 = 2.0 * (tol1 = tol * fabs(x) + ZEPS);

                if (fabs(x - xm) <= (tol2 - 0.5 * (b - a))) {
                    return x;
                }
                if (fabs(e) > tol1) {
                    r = (x - w) * (fx - fv);
                    q = (x - v) * (fx - fw);
                    p = (x - v) * q - (x - w) * r;
                    q = 2.0 * (q - r);

                    if (q > 0.0) p = -p;

                    q = fabs(q);
                    etemp = e;
                    e = d;

                    if (fabs(p) >= fabs(0.5 * q * etemp) || p <= q * (a - x)
						|| p >= q * (b - x)) {
                        d = CGOLD * (e = (x >= xm ? a - x : b - x));
                    } else {
                        d = p / q;
                        u = x + d;
                        if (u - a < tol2 || b - u < tol2) {
                            d=SIGN(tol1,xm-x);
                        }
                    }
                } else {
                    d = CGOLD * (e = (x >= xm ? a - x : b - x));
                }

                u = (fabs(d) >= tol1 ? x + d : x + SIGN(tol1, d));
                for (boost::uint16_t i = 0; i < m_numFree; ++i) {
                    potentialWeights[i] = p_weights[i] + p_trainingDirection[i] * u;
                }
                fu = m_objective->potentialError(potentialWeights, p_targets);

                if (fu <= fx) {
                    if (u >= x) {
                        a = x;
                    } else {
                        b = x;
                    }

                    shft3(v, w, x, u);
                    shft3(fv, fw, fx, fu);
                } else {
                    if (u < x) {
                        a = u;
                    } else {
                        b = u;
                    }

                    if (fu <= fw || w == x) {
                        v = w;
                        w = u;
                        fv = fw;
                        fw = fu;
                    } else if (fu <= fv || v == x || v == w) {
                        v = u;
                        fv = fu;
                    }
                }
            }

            return x;
        }


    NeuralNetwork m_nnet;
    Objective m_objective;
    double m_errtol;
    double m_optimalTrainingRate;
    double m_initialTrainingRate;
    bool m_isFirst;
    boost::uint16_t m_iterMax;

    boost::uint16_t m_numFree;
    double m_optimalTrainingRateTol;

    DoubleSA m_gradientOld;
    DoubleSA m_trainingDirection;

};


}
}

#endif
