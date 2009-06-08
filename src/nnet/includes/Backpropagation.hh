// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a spevectoral exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file Backpropagation.hh
 * Declaration/Implementation of the backpropagation neuro network training algorithm.
 */
#ifndef __BACKPROPAGATION_HH__
#define __BACKPROPAGATION_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <cstring>

#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>


typedef boost::shared_array <double> DoubleSA;
typedef boost::shared_array <DoubleSA> DoubleSM;


namespace des
{
namespace nnet
{

#define LEARNING_RATE   0.001
#define MOMENTUM        0.9
#define ERRTOL          1e-6


template <class NeuralNetwork,
          class Objective>
class Backpropagation
{
public:
    Backpropagation(NeuralNetwork p_nnet, Objective p_objective)
        : m_nnet(p_nnet), m_objective(p_objective),
          m_learningRate(LEARNING_RATE), m_momentum(MOMENTUM), m_errtol(ERRTOL)
        {
            init();
        }

    Backpropagation(NeuralNetwork p_nnet, Objective p_objective,
                    double p_learningRate, double p_momentum, double p_errtol)
        : m_nnet(p_nnet), m_objective(p_objective),
          m_learningRate(p_learningRate), m_momentum(p_momentum), m_errtol(p_errtol)
        {
            init();
        }

    ~Backpropagation()
        {}

    void train(DoubleSA p_targets)
        {
            // don't start the training if the error is small enough
            if (m_objective->addError(p_targets, m_nnet->getOutputNeurons(), m_nnet->getNumOutputs())
                < m_errtol) {
                return;
            }

            m_objective->calc_gradient(p_targets);

            for (boost::uint16_t k = 0; k < m_nnet->getNumOutputs(); ++k) {
                for (boost::uint16_t j = 0; j <= m_nnet->getNumHidden(); ++j) {
                    m_correctionsHiddenOutput[j][k] =
                        m_learningRate * m_objective->getGradientHiddenOutput(j, k)
                        + m_momentum * m_correctionsHiddenOutput[j][k];
                }
            }

            for (boost::uint16_t j = 0; j < m_nnet->getNumHidden(); ++j) {
                for (boost::uint16_t i = 0; i <= m_nnet->getNumInputs(); ++i) {
                    m_correctionsInputHidden[i][j] =
                        m_learningRate * m_objective->getGradientInputHidden(i, j)
                        + m_momentum * m_correctionsInputHidden[i][j];
                }
            }

            updateWeights();
        }


private:
    Backpropagation()
        {}

    void init()
        {
            // initialise the corrections
            m_correctionsInputHidden = DoubleSM(new DoubleSA[m_nnet->getNumInputs() + 1]);
            for (boost::uint16_t i = 0; i <= m_nnet->getNumInputs(); ++i) {
                m_correctionsInputHidden[i] = DoubleSA(new double[m_nnet->getNumHidden()]);

                for (boost::uint16_t j = 0; j < m_nnet->getNumHidden(); ++j) {
                    m_correctionsInputHidden[i][j] = 0.0;
                }
            }

            m_correctionsHiddenOutput = DoubleSM(new DoubleSA[m_nnet->getNumHidden() + 1]);
            for (boost::uint16_t i = 0; i <= m_nnet->getNumHidden(); ++i) {
                m_correctionsHiddenOutput[i] = DoubleSA(new double[m_nnet->getNumOutputs()]);

                for (boost::uint16_t j = 0; j < m_nnet->getNumOutputs(); ++j) {
                    m_correctionsHiddenOutput[i][j] = 0.0;
                }
            }
        }

    void updateWeights()
        {
            for (boost::uint16_t i = 0; i <= m_nnet->getNumInputs(); ++i) {
                for (boost::uint16_t j = 0; j < m_nnet->getNumHidden(); ++j) {
                    m_nnet->addWeightInputHidden(i, j, m_correctionsInputHidden[i][j]);
                }
            }

            for (boost::uint16_t j = 0; j <= m_nnet->getNumHidden(); ++j) {
                for (boost::uint16_t k = 0; k < m_nnet->getNumOutputs(); ++k) {
                    m_nnet->addWeightHiddenOutput(j, k, m_correctionsHiddenOutput[j][k]);
                }
            }
        }


    NeuralNetwork m_nnet;
    Objective m_objective;

    double m_learningRate;
    double m_momentum;
    double m_errtol;

    DoubleSM m_correctionsInputHidden;
    DoubleSM m_correctionsHiddenOutput;

};


}
}

#endif
