#include "Sense.h"

#include "Swimmer.h"
#include "EntityContainerInterface.h"

#include <QPainter>

Sense::Sense(Swimmer& owner, unsigned sensoryInputCount, unsigned hiddenLayers)
    : owner_(owner)
    , network_(hiddenLayers, sensoryInputCount, NeuralNetwork::InitialWeights::PassThrough)
    , senseToBrainConnector_(sensoryInputCount, owner.GetBrainInputCount())
    , inputs_(sensoryInputCount, 0)
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

