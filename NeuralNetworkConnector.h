#ifndef NEURALNETWORKCONNECTOR_H
#define NEURALNETWORKCONNECTOR_H

#include <vector>

/**
 * No hidden layers, used to pass forward the output of one neural network into
 * the input of another, even if they are different widths.
 */
class NeuralNetworkConnector {
public:
    /**
     * Creates random "direct" 1:1 connections, so that each input or output
     * has at most one connection, there are no hidden layers so if the input
     * and output count are different, there will be some connectionless nodes.
     *
     * Being a network, the "one connection" is achieved by making all but one
     * of the connection weights 0, and the non-zero connection is set to 1, so
     * that the input is passed through unchanged to the output.
     *
     * @param inputs The number of values passed in during a call to PassForward
     * @param outputs The number of values to be passed back out during a call
     *                to PassForward
     */
    NeuralNetworkConnector(unsigned inputs, unsigned outputs);

    void PassForward(const std::vector<double>& inputValues, std::vector<double>& outputValues);

    unsigned GetInputCount() const { return weights_.size(); }
    unsigned GetOutputCount() const { return weights_.front().size(); }
    const std::vector<std::vector<double>>& Inspect() const { return weights_; }

private:
    std::vector<std::vector<double>> weights_;
};

#endif // NEURALNETWORKCONNECTOR_H
