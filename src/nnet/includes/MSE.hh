// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a spevectoral exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file MSE.hh
 * Declaration/Implementation of the objective function for neural networks. Here
 * the mean squared error is implemented.
 */
#ifndef __MSE_HH__
#define __MSE_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>


typedef boost::shared_array <double> DoubleSA;


namespace des { namespace nnet {


/** @class MSE
 * This class implements the mean squared error objective function.
 *
 * @author Dominik Dahlem
 */
template <class NeuralNetwork,
          class Activation,
          class ActivationOutput = Activation>
class MSE
{
public:
    MSE()
        {}

    MSE(NeuralNetwork p_nnet)
        : m_nnet(p_nnet)
        {
            init();
        }

    ~MSE()
        {}

    double potentialError(DoubleSA p_weights, DoubleSA p_targets)
        {
            DoubleSA origWeights = m_nnet->getWeights();

            // set the potential weights
            boost::uint16_t index = 0;

            for (boost::uint16_t j = 0; j < m_nnet->getNumHidden(); ++j) {
                for (boost::uint16_t i = 0; i <= m_nnet->getNumInputs(); ++i) {
                    m_nnet->setWeightInputHidden(i, j, p_weights[index]);
                    index++;
                }
            }

            for (boost::uint16_t k = 0; k < m_nnet->getNumOutputs(); ++k) {
                for (boost::uint16_t j = 0; j <= m_nnet->getNumHidden(); ++j) {
                    m_nnet->setWeightHiddenOutput(j, k, p_weights[index]);
                    index++;
                }
            }

            // present the old input to the network to get new output values
            DoubleSA origInputs = m_nnet->getInputNeurons();
            DoubleSA newOutputs = m_nnet->present(origInputs);

            // calc the error
            double error = MSE::error(p_targets, newOutputs, m_nnet->getNumOutputs());

            // reset the weights to the old values
            index = 0;

            for (boost::uint16_t j = 0; j < m_nnet->getNumHidden(); ++j) {
                for (boost::uint16_t i = 0; i <= m_nnet->getNumInputs(); ++i) {
                    m_nnet->setWeightInputHidden(i, j, origWeights[index]);
                    index++;
                }
            }

            for (boost::uint16_t k = 0; k < m_nnet->getNumOutputs(); ++k) {
                for (boost::uint16_t j = 0; j <= m_nnet->getNumHidden(); ++j) {
                    m_nnet->setWeightHiddenOutput(j, k, origWeights[index]);
                    index++;
                }
            }

            return error;
        }

    static double error(DoubleSA p_targets, DoubleSA p_outputs, boost::uint16_t p_size)
        {
            double d, sum = 0.0;

            for (boost::uint16_t i = 0; i < p_size; ++i) {
                d = p_targets[i] - p_outputs[i];
                sum += d * d;
            }

            return sum / static_cast<double> (p_size);
        }

    void calc_gradient(DoubleSA p_targets)
        {
            for (boost::uint16_t k = 0; k < m_nnet->getNumOutputs(); ++k) {
                m_deltaOutput[k] = getOutputDelta(p_targets[k], k);

                for (boost::uint16_t j = 0; j <= m_nnet->getNumHidden(); ++j) {
                    m_gradientHiddenOutput[j][k] = m_deltaOutput[k] * m_nnet->getHiddenNeuron(j);
                }
            }

            for (boost::uint16_t j = 0; j < m_nnet->getNumHidden(); ++j) {
                m_deltaHidden[j] = getHiddenDelta(j);

                for (boost::uint16_t i = 0; i <= m_nnet->getNumInputs(); ++i) {
                    m_gradientInputHidden[i][j] = m_deltaHidden[j] * m_nnet->getInputNeuron(i);
                }
            }
        }

    inline
    double getGradientHiddenOutput(boost::uint16_t j, boost::uint16_t k)
        {
            return m_gradientHiddenOutput[j][k];
        }

    inline
    double getGradientInputHidden(boost::uint16_t i, boost::uint16_t j)
        {
            return m_gradientInputHidden[i][j];
        }

    DoubleSA getGradient()
        {
            boost::uint16_t index = 0;

            DoubleSA gradient = DoubleSA(new double[m_nnet->getNumFree()]);

            for (boost::uint16_t j = 0; j < m_nnet->getNumHidden(); ++j) {
                for (boost::uint16_t i = 0; i <= m_nnet->getNumInputs(); ++i) {
                    gradient[index] = m_gradientInputHidden[i][j];
                    index++;
                }
            }

            for (boost::uint16_t k = 0; k < m_nnet->getNumOutputs(); ++k) {
                for (boost::uint16_t j = 0; j <= m_nnet->getNumHidden(); ++j) {
                    gradient[index] = m_gradientHiddenOutput[j][k];
                    index++;
                }
            }

            return gradient;
        }


private:
    MSE(const MSE&)
        {}

    void init()
        {
            // initialise the gradients
            m_gradientInputHidden = DoubleSM(new DoubleSA[m_nnet->getNumInputs() + 1]);
            for (boost::uint16_t i = 0; i <= m_nnet->getNumInputs(); ++i) {
                m_gradientInputHidden[i] = DoubleSA(new double[m_nnet->getNumHidden()]);

                for (boost::uint16_t j = 0; j < m_nnet->getNumHidden(); ++j) {
                    m_gradientInputHidden[i][j] = 0.0;
                }
            }

            m_gradientHiddenOutput = DoubleSM(new DoubleSA[m_nnet->getNumHidden() + 1]);
            for (boost::uint16_t i = 0; i <= m_nnet->getNumHidden(); ++i) {
                m_gradientHiddenOutput[i] = DoubleSA(new double[m_nnet->getNumOutputs()]);

                for (boost::uint16_t j = 0; j < m_nnet->getNumOutputs(); ++j) {
                    m_gradientHiddenOutput[i][j] = 0.0;
                }
            }

            // initialise the deltas
            m_deltaHidden = DoubleSA(new double[m_nnet->getNumHidden() + 1]);
            memset(m_deltaHidden.get(), 0, (m_nnet->getNumHidden() + 1) * sizeof(double));

            m_deltaOutput = DoubleSA(new double[m_nnet->getNumOutputs() + 1]);
            memset(m_deltaOutput.get(), 0, (m_nnet->getNumOutputs() + 1) * sizeof(double));
        }

    inline double getOutputDelta(double p_target, boost::uint16_t p_k)
        {
            double output = m_nnet->getOutputNeuron(p_k);
            double net = 0.0;

            for (boost::uint16_t j = 0; j <= m_nnet->getNumHidden(); ++j) {
                net += m_nnet->getWeightHiddenOutput(j, p_k) * m_nnet->getHiddenNeuron(j);
            }

            return ActivationOutput::deriv(net) * (p_target - output);
        }

    double getHiddenDelta(boost::uint16_t j)
        {
            double sum = 0.0;
            double net = 0.0;

            for (boost::uint16_t i = 0; i <= m_nnet->getNumInputs(); ++i) {
                net += m_nnet->getWeightInputHidden(i, j) * m_nnet->getInputNeuron(i);
            }

            for (boost::uint16_t k = 0; k < m_nnet->getNumOutputs(); ++k) {
                sum += m_nnet->getWeightHiddenOutput(j, k) * m_deltaOutput[k];
            }

            return Activation::deriv(net) * sum;
        }


    NeuralNetwork m_nnet;

    DoubleSM m_gradientInputHidden;
    DoubleSM m_gradientHiddenOutput;

    DoubleSA m_deltaHidden;
    DoubleSA m_deltaOutput;

};


}
}

#endif /* __MSE_HH__ */
