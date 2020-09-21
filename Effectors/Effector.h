#ifndef EFFECTOR_H
#define EFFECTOR_H

#include "NeuralNetworkConnector.h"

#include <vector>

class Swimmer;
class EntityContainerInterface;
class UniverseInfoStructRef;
class QPainter;

// TODO consider thet effectors may also want to have hidden layers, like senses
class Effector {
public:
    Effector(Swimmer& owner, unsigned actionsCount);

    virtual void Draw(QPainter& paint) const = 0;
    virtual void Tick(const std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseInfoStructRef& environment) final;

    const std::vector<double>& GetPreviousActionValues() { return actionValues_; }

protected:
    Swimmer& owner_;

private:
    NeuralNetworkConnector brainToEffectorConnector_;
    std::vector<double> actionValues_;

    virtual void PerformActions(const std::vector<double>& actionValues, const EntityContainerInterface& entities, const UniverseInfoStructRef& environment) = 0;
};

#endif // EFFECTOR_H
