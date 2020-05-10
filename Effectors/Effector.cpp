#include "Effector.h"

#include "Swimmer.h"

Effector::Effector(Swimmer& owner, unsigned actionsCount)
    : owner_(owner)
    , brainToEffectorConnector_(owner_.GetBrainInputCount(), actionsCount)
    , actionValues_(actionsCount, 0)
{
}

void Effector::Tick(const std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseInfoStructRef& environment)
{
    std::fill(std::begin(actionValues_), std::end(actionValues_), 0);
    brainToEffectorConnector_.PassForward(inputs, actionValues_);
    PerformActions(actionValues_, entities, environment);
}
