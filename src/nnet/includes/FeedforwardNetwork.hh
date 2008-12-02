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
#include <iostream>

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

template <class Activation, class ActivationOutput = Activation>
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
            m_input_neurons[m_num_inputs] = 1;

            m_hidden_neurons = DoubleSA(new double[m_num_hidden + 1]);
            memset(m_hidden_neurons.get(), 0, (m_num_hidden + 1) * sizeof(double));
            m_hidden_neurons[m_num_hidden] = 1;

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
                m_output_neurons[k] = ActivationOutput::activate(m_output_neurons[k]);
            }

            DoubleSA output = DoubleSA(new double[m_num_outputs]);
            for (boost::uint16_t k = 0; k < m_num_outputs; ++k) {
                output[k] = m_output_neurons[k];
            }

            return output;
        }

    DoubleSA getOutputNeurons()
        {
            DoubleSA output = DoubleSA(new double[m_num_outputs]);
            for (boost::uint16_t k = 0; k < m_num_outputs; ++k) {
                output[k] = m_output_neurons[k];
            }

            return output;
        }

    inline
    double getInputNeuron(boost::uint16_t i) const
        {
            return m_input_neurons[i];
        }

    inline
    double getHiddenNeuron(boost::uint16_t j) const
        {
            return m_hidden_neurons[j];
        }

    inline
    double getOutputNeuron(boost::uint16_t k) const
        {
            return m_output_neurons[k];
        }

    inline
    double getWeightInputHidden(boost::uint16_t i, boost::uint16_t j) const
        {
            return m_weights_inputHidden[i][j];
        }

    inline
    double getWeightHiddenOutput(boost::uint16_t j, boost::uint16_t k) const
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
    boost::uint16_t getNumInputs() const
        {
            return m_num_inputs;
        }

    inline
    boost::uint16_t getNumHidden() const
        {
            return m_num_hidden;
        }

    inline
    boost::uint16_t getNumOutputs() const
        {
            return m_num_outputs;
        }

    inline
    boost::uint16_t getNumFree() const
        {
            return m_num_hidden + m_num_outputs
                + m_num_inputs * m_num_hidden
                + m_num_hidden * m_num_outputs;
        }

    friend std::ostream& operator <<(
        std::ostream &p_os,
        const FeedforwardNetwork <Activation, ActivationOutput> &p_nnet)
        {
            p_os << "** Hidden Layer **" << std::endl;

            for (boost::uint16_t j = 0; j < p_nnet.getNumHidden();  ++j) {
                p_os << j << ". Neuron Value : " << p_nnet.getHiddenNeuron(j) << std::endl;

                for (boost::uint16_t i = 0; i < p_nnet.getNumInputs(); ++i) {
                    p_os << "\t" << i << ". input value : " << p_nnet.getInputNeuron(i)
                         << " - weight : " << p_nnet.getWeightInputHidden(i, j) << std::endl;
                }
                p_os << "\t" "Bias value : " << p_nnet.getInputNeuron(p_nnet.getNumInputs())
                     << " - weight : " << p_nnet.getWeightInputHidden(p_nnet.getNumInputs(), j) << std::endl;
            }

            p_os << "** Output Layer **" << std::endl;

            for (boost::uint16_t k = 0; k < p_nnet.getNumOutputs();  ++k) {
                p_os << k << ". Neuron Value : " << p_nnet.getOutputNeuron(k) << std::endl;

                for (boost::uint16_t j = 0; j < p_nnet.getNumHidden(); ++j) {
                    p_os << "\t" << j << ". input value : " << p_nnet.getHiddenNeuron(j)
                         << " - weight : " << p_nnet.getWeightHiddenOutput(j, k) << std::endl;
                }
                p_os << "\t" << "Bias value : " << p_nnet.getHiddenNeuron(p_nnet.getNumHidden())
                     << " - weight : " << p_nnet.getWeightHiddenOutput(p_nnet.getNumHidden(), k) << std::endl;
            }

            p_os << std::endl;

            return p_os;
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

}
}



#endif /* __FEEDFORWARDNETWORK_HH__ */
