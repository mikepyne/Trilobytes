#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "Random.h"

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <vector>
#include <algorithm>
#include <assert.h>

/**
 * A basic NeuralNetwork with no backward propogation. The sigma function
 * operates between 0.0 and 0.1.
 */
class NeuralNetwork {
public:
    // I don't usually like typedefs, but here we can collapse a complex 3D array into manageable types without creating a bunch of structs
    using InputWeight = double;
    using Node = std::vector<InputWeight>;
    using Layer = std::vector<Node>;

    /**
     * Creates a rectangular network of the specified width and height, with
     * random edge weights between 0.0 and 1.0.
     */
    NeuralNetwork(unsigned layerCount, unsigned width);

    /**
     * Inputs should be between 0.0 and 1.0 inclusive. Returns the final node
     * values.
     *
     * OPTIMISATION Perhaps nodes storing their value instead of populating
     * vectors could improve performance by reducting copying
     */
    const std::vector<double>& ForwardPropogate(const std::vector<double>& inputs);

private:
    std::vector<Layer> layers_;
    std::vector<double> outputs_;
};

#endif // NEURALNETWORK_H
