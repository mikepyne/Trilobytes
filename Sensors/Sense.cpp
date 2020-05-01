#include "Sense.h"

#include "EntityContainerInterface.h"

#include <QPainter>

Sense::Sense(Entity& owner, unsigned inputs, unsigned hiddenLayers, unsigned outputs)
    : owner_(owner)
    , network_(hiddenLayers, inputs, NeuralNetwork::InitialWeights::PassThrough)
    , senseToBrainConnector_(inputs, outputs)
    , inputs_(inputs, 0)
{
}

Sense::~Sense()
{
}

void Sense::Draw(QPainter& /*paint*/) const
{
    // Nothing to draw
}

void Sense::Tick(std::vector<double>& outputs, const EntityContainerInterface& entities, const Sense::UniverseInfoStructRef& environment)
{
    PrimeInputs(inputs_, entities, environment);
    network_.ForwardPropogate(inputs_);
    senseToBrainConnector_.PassForward(inputs_, outputs);
}

