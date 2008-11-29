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


typedef boost::shared_array <double> DoubleSA;
typedef boost::shared_array <DoubleSA> DoubleSM;


namespace des
{
namespace nnet
{

#define LEARNING_RATE   0.001
#define MOMENTUM        0.9


template <class NeuralNetwork, class Activation>
class Backpropagation
{
public:
    Backpropagation(NeuralNetwork p_nnet)
        : m_nnet(p_nnet), m_learningRate(LEARNING_RATE), m_momentum(MOMENTUM)
        {
            init();
        }

    Backpropagation(NeuralNetwork p_nnet, double p_learningRate, double p_momentum)
        : m_nnet(p_nnet), m_learningRate(p_learningRate), m_momentum(p_momentum)
        {
            init();
        }

    ~Backpropagation()
        {}

    inline double getOutputGradient(double p_target, double p_output)
        {
            return Activation::deriv(p_output) * (p_target - p_output);
        }

    double getHiddenGradient(boost::uint16_t j)
        {
            double sum = 0.0;

            for (boost::uint16_t k = 0; k < m_nnet->getNumOutputs(); ++k) {
                sum += m_nnet->getWeightHiddenOutput(j, k) * m_gradientOutput[k];
            }

            return Activation::deriv(m_nnet->getHiddenNeuron(j)) * sum;
        }

    void train(DoubleSA p_targets)
        {
            for (boost::uint16_t k = 0; k < m_nnet->getNumOutputs(); ++k) {
                m_gradientOutput[k] = getOutputGradient(
                    p_targets[k], m_nnet->getOutputNeuron(k));

                for (boost::uint16_t j = 0; j <= m_nnet->getNumHidden(); ++j) {
                    m_deltaHiddenOutput[j][k] =
                        m_learningRate * m_nnet->getHiddenNeuron(j) * m_gradientOutput[k]
                        + m_momentum * m_deltaHiddenOutput[j][k];
                }
            }

            for (boost::uint16_t j = 0; j < m_nnet->getNumHidden(); ++j) {
                m_gradientHidden[j] = getHiddenGradient(j);

                for (boost::uint16_t i = 0; i <= m_nnet->getNumInputs(); ++i) {
                    m_deltaInputHidden[i][j] =
                        m_learningRate * m_nnet->getInputNeuron(i) * m_gradientHidden[j]
                        + m_momentum * m_deltaInputHidden[i][j];
                }
            }

            updateWeights();
        }


private:
    Backpropagation()
        {}

    void init()
        {
            // initialise the deltas
            m_deltaInputHidden = DoubleSM(new DoubleSA[m_nnet->getNumInputs() + 1]);
            for (boost::uint16_t i = 0; i <= m_nnet->getNumInputs(); ++i) {
                m_deltaInputHidden[i] = DoubleSA(new double[m_nnet->getNumHidden()]);

                for (boost::uint16_t j = 0; j < m_nnet->getNumHidden(); ++j) {
                    m_deltaInputHidden[i][j] = 0.0;
                }
            }

            m_deltaHiddenOutput = DoubleSM(new DoubleSA[m_nnet->getNumHidden() + 1]);
            for (boost::uint16_t i = 0; i <= m_nnet->getNumHidden(); ++i) {
                m_deltaHiddenOutput[i] = DoubleSA(new double[m_nnet->getNumOutputs()]);

                for (boost::uint16_t j = 0; j < m_nnet->getNumOutputs(); ++j) {
                    m_deltaHiddenOutput[i][j] = 0.0;
                }
            }

            // initialise the gradients
            m_gradientHidden = DoubleSA(new double[m_nnet->getNumHidden() + 1]);
            memset(m_gradientHidden.get(), 0, (m_nnet->getNumHidden() + 1) * sizeof(double));

            m_gradientOutput = DoubleSA(new double[m_nnet->getNumOutputs() + 1]);
            memset(m_gradientOutput.get(), 0, (m_nnet->getNumOutputs() + 1) * sizeof(double));
        }

    void updateWeights()
        {
            for (boost::uint16_t i = 0; i <= m_nnet->getNumInputs(); ++i) {
                for (boost::uint16_t j = 0; j < m_nnet->getNumHidden(); ++j) {
                    m_nnet->addWeightInputHidden(i, j, m_deltaInputHidden[i][j]);
                    m_deltaInputHidden[i][j] = 0.0;
                }
            }

            for (boost::uint16_t j = 0; j <= m_nnet->getNumHidden(); ++j) {
                for (boost::uint16_t k = 0; k < m_nnet->getNumOutputs(); ++k) {
                    m_nnet->addWeightHiddenOutput(j, k, m_deltaHiddenOutput[j][k]);
                    m_deltaHiddenOutput[j][k] = 0.0;
                }
            }
        }


    NeuralNetwork m_nnet;

    double m_learningRate;
    double m_momentum;

    DoubleSM m_deltaInputHidden;
    DoubleSM m_deltaHiddenOutput;

    DoubleSA m_gradientHidden;
    DoubleSA m_gradientOutput;

};


}
}

#endif
