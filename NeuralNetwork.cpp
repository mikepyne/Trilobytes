#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(unsigned layerCount, unsigned width, NeuralNetwork::InitialWeights initialWeights)
    : NeuralNetwork(initialWeights == InitialWeights::Random ? CreateRandomLayers(layerCount, width) : CreatePassThroughLayers(layerCount, width))
{
}

NeuralNetwork::NeuralNetwork(std::vector<NeuralNetwork::Layer>&& layers)
    : layers_(std::move(layers))
{
}

void NeuralNetwork::ForwardPropogate(std::vector<double>& toPropogate) const
{
    static std::vector<double> previousNodeValues;

    // about to swap with previousNodeValues so we can return outputs at the end
    // also allows to skip propogation when no hidden layers
    for (auto& layer : layers_) {
        std::swap(toPropogate, previousNodeValues);
        // We'll reuse this vector for the output of each layer
        toPropogate.assign(layer.size(), 0.0);

        size_t nodeIndex = 0;
        for (auto& node : layer) {
            double nodeValue = 0.0;
            size_t edgeIndex = 0;
            for (auto& edge : node) {
                nodeValue += edge * previousNodeValues.at(edgeIndex);
                ++edgeIndex;
            }
            // tanh is our sigma function
            toPropogate.at(nodeIndex) = std::tanh(nodeValue);
            nodeIndex++;
        }
    }
}

std::vector<size_t> NeuralNetwork::GetLayerWidths() const
{
    std::vector<size_t> layerWidths;
    layerWidths.resize(layers_.size());
    for (auto& layer : layers_) {
        layerWidths.push_back(layer.size());
    }
    return layerWidths;
}

void NeuralNetwork::ForEach(const std::function<void (unsigned, unsigned, const NeuralNetwork::Node&)>& perNode) const
{
    unsigned layerIndex = 1;
    for (const auto& layer : layers_) {
        unsigned nodeIndex = 0;
        for (const auto& node : layer) {
            perNode(nodeIndex, layerIndex, node);
            nodeIndex++;
        }
        layerIndex++;
    }
}

std::shared_ptr<NeuralNetwork> NeuralNetwork::Mutated() const
{
    size_t layerCount = layers_.size();
    std::vector<Layer> layers;
    for (auto& layer : layers_) {
        layers.push_back(Layer());
        for (auto& node : layer) {
            layers.back().push_back(Node());
            for (auto& edge : node) {
                double newEdge = edge;
                // i.e average 3 mutations per child
                if (Random::Number(0u, layerCount * layer.size() * layer.size()) < 3) {
                    newEdge += Random::Gaussian(0.0, 1.0);
                }
                layers.back().back().push_back(newEdge);
            }
        }
    }
    return std::make_shared<NeuralNetwork>(std::move(layers));
}



std::vector<NeuralNetwork::Layer> NeuralNetwork::CreateRandomLayers(unsigned layerCount, unsigned width)
{
    std::vector<Layer> layers(layerCount, Layer(width, Node(width)));

    // First layer doesn't need input weights, as it will be assigned a value by the ForwardPropogate() func
    for (auto& layer : layers) {
        for (auto& node : layer) {
            double mean = 0.75;
            double stdDev = 0.25;
            node = Random::DualPeakGaussians(width, -mean, stdDev, mean, stdDev);
        }
    }

    return layers;
}

std::vector<NeuralNetwork::Layer> NeuralNetwork::CreatePassThroughLayers(unsigned layerCount, unsigned width)
{
    std::vector<Layer> layers(layerCount, Layer(width, Node(width)));

    // First layer doesn't need input weights, as it will be assigned a value by the ForwardPropogate() func
    for (auto& layer : layers) {
        unsigned nodeColumn = 0;
        for (auto& node : layer) {
            for (unsigned inputColumn = 0; inputColumn < node.size(); inputColumn++) {
                node.at(inputColumn) = (inputColumn == nodeColumn ? 1.0 : 0.0);
            }
            nodeColumn++;
        }
    }

    return layers;
}
