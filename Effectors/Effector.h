#ifndef EFFECTOR_H
#define EFFECTOR_H

#include "NeuralNetwork.h"
#include "NeuralNetworkConnector.h"

#include <vector>

class Swimmer;
class EntityContainerInterface;
class UniverseInfoStructRef;
class QPainter;

class Effector {
public:
    Effector(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, Swimmer& owner);
    virtual ~Effector();

    virtual std::string_view GetName() const = 0;

    virtual void Draw(QPainter& paint) const = 0;
    virtual void Tick(const std::vector<double>& inputs, const EntityContainerInterface& entities) final;

    unsigned GetInputCount() const { return network_->GetInputCount(); }

    const NeuralNetwork& Inspect() const { return *network_; }
    const NeuralNetworkConnector& InspectConnection() const { return *inputConnections_; }

protected:
    Swimmer& owner_;

private:
    std::shared_ptr<NeuralNetwork> network_;
    std::shared_ptr<NeuralNetworkConnector> inputConnections_;
    std::vector<double> outputs_;

    virtual void PerformActions(const std::vector<double>& actionValues, const EntityContainerInterface& entities) = 0;
};

#endif // EFFECTOR_H
