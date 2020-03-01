#include "SenseEntityPresence.h"

SenseEntityPresence::CustomFilter SenseEntityPresence::MakeDefaultFilter(double detectionQuantity)
{
    return {
        1,
        0,
        "Entity",
        [=](Entity& e) -> std::optional<std::pair<size_t, double>>
        {
            return std::make_pair(0u, detectionQuantity);
        }
    };
}

void SenseEntityPresence::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const Sense::UniverseInfoStructRef& environment)
{
    for (double& input : inputs) {
        input = 0.0;
    }

    entities.ForEachIn(Circle{ owner_.GetX() + xOffset_, owner_.GetY() + yOffset_, range_ }, [&](Entity& e)
    {
        if (auto optional = filter_(e); optional.has_value()) {
            auto [ index, value ] = optional.value();
            inputs[index] += value;
        }
    });
}
