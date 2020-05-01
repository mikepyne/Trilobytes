#ifndef NEURALNETWORKCONNECTOR_H
#define NEURALNETWORKCONNECTOR_H

#include "Utils.h"
#include "Random.h"

#include <vector>
#include <assert.h>

class NeuralNetworkConnector {
public:
    /**
     * Connects each input to a random output and zeroes all  other connections.
     */
    NeuralNetworkConnector(unsigned inputs, unsigned outputs);

    void PassForward(const std::vector<double>& inputValues, std::vector<double>& outputValues);

private:
    std::vector<std::vector<double>> weights_;
};

#endif // NEURALNETWORKCONNECTOR_H
