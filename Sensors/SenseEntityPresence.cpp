#include "SenseEntityPresence.h"

SenseEntityPresence::SenseEntityPresence(Entity& owner, double xOffset, double yOffset, double range)
    : SenseEntityPresence(owner, xOffset, yOffset, range, 1, 0, "EntityPresenceSensor", [](Entity&) -> std::optional<size_t> { return {0}; })
{
}

SenseEntityPresence::SenseEntityPresence(Entity& owner, double xOffset, double yOffset, double range, unsigned inputs, unsigned hiddenLayers, const std::string& name, std::function<std::optional<size_t> (Entity&)>&& filter)
    : Sense(owner, inputs, hiddenLayers)
    , name_(name)
    , xOffset_(xOffset)
    , yOffset_(yOffset)
    , range_(range)
    , detectionQuantity_(1.0)
    , filter_(std::move(filter))
{
}

void SenseEntityPresence::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const Sense::UniverseInfoStructRef& environment)
{
    for (double& input : inputs) {
        input = 0.0;
    }


    entities.ForEachIn(Circle{ owner_.GetX() + xOffset_, owner_.GetY() + yOffset_, range_ }, [&](Entity& e)
    {
        if (auto index = filter_(e); index.has_value()) {
            inputs[index.value()] += detectionQuantity_;
        }
    });
}
