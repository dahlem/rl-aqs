// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a spevectoral exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file FeedforwardNetwork.hh
 * Declaration/Implementation of the network for neural networks
 */
#ifndef __FEEDFORWARDNETWORK_HH__
#define __FEEDFORWARDNETWORK_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <cmath>
#include <cstring>

#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>

#include <gsl/gsl_rng.h>

#include "CRN.hh"
namespace dsample = des::sampling;


typedef boost::shared_array <double> DoubleSA;
typedef boost::shared_array <DoubleSA> DoubleSM;


namespace des
{
namespace nnet
{

template <class Activation, class Objective>
class FeedforwardNetwork
{
public:
    FeedforwardNetwork(boost::uint16_t p_num_inputs, boost::uint16_t p_num_hidden,
                       boost::uint16_t p_num_outputs, boost::uint32_t p_uniform_idx)
        : m_num_inputs(p_num_inputs), m_num_hidden(p_num_hidden),
          m_num_outputs(p_num_outputs)
        {
            // initialise the neurons
            m_input_neurons = DoubleSA(new double[m_num_inputs + 1]);
            memset(m_input_neurons.get(), 0, (m_num_inputs + 1) * sizeof(double));
            m_input_neurons[m_num_inputs] = -1;

            m_hidden_neurons = DoubleSA(new double[m_num_hidden + 1]);
            memset(m_hidden_neurons.get(), 0, (m_num_hidden + 1) * sizeof(double));
            m_hidden_neurons[m_num_hidden] = -1;

            m_output_neurons = DoubleSA(new double[m_num_outputs]);
            memset(m_output_neurons.get(), 0, m_num_outputs * sizeof(double));

            // initialise the weights
            dsample::tGslRngSP uniform_rng = dsample::CRN::getInstance().get(p_uniform_idx - 1);

            // [-range_*;+range_*]
            double range_hidden = 1 / sqrt(static_cast<double> (m_num_inputs));
            double range_output = 1 / sqrt(static_cast<double> (m_num_hidden));

            m_weights_inputHidden = DoubleSM(new DoubleSA[m_num_inputs + 1]);
            for (boost::uint16_t i = 0; i <= m_num_inputs; ++i) {
                m_weights_inputHidden[i] = DoubleSA(new double[m_num_hidden]);

                for (boost::uint16_t j = 0; j < m_num_hidden; ++j) {
                    m_weights_inputHidden[i][j] =
                        -range_hidden + gsl_rng_uniform(uniform_rng.get()) * 2 * range_hidden;
                }
            }

            m_weights_hiddenOutput = DoubleSM(new DoubleSA[m_num_hidden + 1]);
            for (boost::uint16_t i = 0; i <= m_num_hidden; ++i) {
                m_weights_hiddenOutput[i] = DoubleSA(new double[m_num_outputs]);

                for (boost::uint16_t j = 0; j < m_num_outputs; ++j) {
                    m_weights_hiddenOutput[i][j] =
                        -range_output + gsl_rng_uniform(uniform_rng.get()) * 2 * range_output;
                }
            }
        }

    ~FeedforwardNetwork()
        {}

    DoubleSA present(DoubleSA p_inputs)
        {
            // set the value of the input neurons
            for (boost::uint16_t i = 0; i < m_num_inputs; ++i) {
                m_input_neurons[i] = p_inputs[i];
            }

            // calculate the hidden layer values
            for (boost::uint16_t j = 0; j < m_num_hidden; ++j) {
                // clear value
                m_hidden_neurons[j] = 0.0;

                for (boost::uint16_t i = 0; i <= m_num_inputs; ++i) {
                    m_hidden_neurons[j] += m_input_neurons[i] * m_weights_inputHidden[i][j];
                }
                m_hidden_neurons[j] = Activation::activate(m_hidden_neurons[j]);
            }

            // calculate the output layer values
            for (boost::uint16_t k = 0; k < m_num_outputs; ++k) {
                // clear value
                m_output_neurons[k] = 0.0;

                for (boost::uint16_t j = 0; j <= m_num_hidden; ++j) {
                    m_output_neurons[k] += m_hidden_neurons[j] * m_weights_hiddenOutput[j][k];
                }
                m_output_neurons[k] = Activation::activate(m_output_neurons[k]);
            }

            DoubleSA output = DoubleSA(new double[m_num_outputs + 1]);
            for (boost::uint16_t k = 0; k < m_num_outputs; ++k) {
                output[k] = m_output_neurons[k];
            }

            return output;
        }

    inline
    double error(DoubleSA p_targets)
        {
            return  Objective::error(p_targets,
                                     m_output_neurons,
                                     m_num_outputs);

        }

    inline
    double getInputNeuron(boost::uint16_t i)
        {
            return m_input_neurons[i];
        }

    inline
    double getHiddenNeuron(boost::uint16_t j)
        {
            return m_hidden_neurons[j];
        }

    inline
    double getOutputNeuron(boost::uint16_t k)
        {
            return m_output_neurons[k];
        }

    inline
    double getWeightHiddenOutput(boost::uint16_t j, boost::uint16_t k)
        {
            return m_weights_hiddenOutput[j][k];
        }

    inline
    void addWeightInputHidden(boost::uint16_t i, boost::uint16_t j, double p_delta)
        {
            m_weights_inputHidden[i][j] += p_delta;
        }

    inline
    void addWeightHiddenOutput(boost::uint16_t j, boost::uint16_t k, double p_delta)
        {
            m_weights_hiddenOutput[j][k] += p_delta;
        }

    inline
    boost::uint16_t getNumInputs()
        {
            return m_num_inputs;
        }

    inline
    boost::uint16_t getNumHidden()
        {
            return m_num_hidden;
        }

    inline
    boost::uint16_t getNumOutputs()
        {
            return m_num_outputs;
        }


private:
    boost::uint16_t m_num_inputs;
    boost::uint16_t m_num_hidden;
    boost::uint16_t m_num_outputs;

    // neurons
    DoubleSA m_input_neurons;
    DoubleSA m_hidden_neurons;
    DoubleSA m_output_neurons;

    // weights
    DoubleSM m_weights_inputHidden;
    DoubleSM m_weights_hiddenOutput;
};


typedef boost::shared_ptr <FeedforwardNetwork <class Activation, class Objective> > FFNetworkSP;

}
}



#endif /* __FEEDFORWARDNETWORK_HH__ */
