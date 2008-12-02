// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
          class Activation,
          class Objective,
          class ActivationOutput = Activation>
class Backpropagation
{
public:
    Backpropagation(NeuralNetwork p_nnet)
        : m_nnet(p_nnet), m_learningRate(LEARNING_RATE), m_momentum(MOMENTUM), m_errtol(ERRTOL)
        {
            init();
        }

    Backpropagation(NeuralNetwork p_nnet, double p_learningRate, double p_momentum, double p_errtol)
        : m_nnet(p_nnet), m_learningRate(p_learningRate), m_momentum(p_momentum), m_errtol(p_errtol)
        {
            init();
        }

    ~Backpropagation()
        {}

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

    void train(DoubleSA p_targets)
        {
            // don't start the training if the error is small enough
            if (Objective::error(p_targets, m_nnet->getOutputNeurons(), m_nnet->getNumOutputs())
                < m_errtol) {
                return;
            }

            for (boost::uint16_t k = 0; k < m_nnet->getNumOutputs(); ++k) {
                m_deltaOutput[k] = getOutputDelta(p_targets[k], k);

                for (boost::uint16_t j = 0; j <= m_nnet->getNumHidden(); ++j) {
                    m_gradientHiddenOutput[j][k] = m_deltaOutput[k] * m_nnet->getHiddenNeuron(j);
                    m_correctionsHiddenOutput[j][k] =
                        m_learningRate * m_gradientHiddenOutput[j][k]
                        + m_momentum * m_correctionsHiddenOutput[j][k];
                }
            }

            for (boost::uint16_t j = 0; j < m_nnet->getNumHidden(); ++j) {
                m_deltaHidden[j] = getHiddenDelta(j);

                for (boost::uint16_t i = 0; i <= m_nnet->getNumInputs(); ++i) {
                    m_gradientInputHidden[i][j] = m_deltaHidden[j] * m_nnet->getInputNeuron(i);
                    m_correctionsInputHidden[i][j] =
                        m_learningRate * m_gradientInputHidden[i][j]
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

            // initialise the deltas
            m_deltaHidden = DoubleSA(new double[m_nnet->getNumHidden() + 1]);
            memset(m_deltaHidden.get(), 0, (m_nnet->getNumHidden() + 1) * sizeof(double));

            m_deltaOutput = DoubleSA(new double[m_nnet->getNumOutputs() + 1]);
            memset(m_deltaOutput.get(), 0, (m_nnet->getNumOutputs() + 1) * sizeof(double));
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

    double m_learningRate;
    double m_momentum;
    double m_errtol;

    DoubleSM m_gradientInputHidden;
    DoubleSM m_gradientHiddenOutput;

    DoubleSM m_correctionsInputHidden;
    DoubleSM m_correctionsHiddenOutput;

    DoubleSA m_deltaHidden;
    DoubleSA m_deltaOutput;

};


}
}

#endif
