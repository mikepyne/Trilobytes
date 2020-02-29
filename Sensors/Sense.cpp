#include "Sense.h"

#include "EntityContainerInterface.h"

#include <QPainter>

Sense::Sense(Entity& owner, unsigned inputs, unsigned hiddenLayers)
    : owner_(owner)
    , network_(hiddenLayers, inputs)
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

const std::vector<double>& Sense::Tick(const EntityContainerInterface& entities, const Sense::UniverseInfoStructRef& environment)
{
    PrimeInputs(inputs_, entities, environment);
    return network_.ForwardPropogate(inputs_);
}

