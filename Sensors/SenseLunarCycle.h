#ifndef SENSELUNARCYCLE_H
#define SENSELUNARCYCLE_H

#include "Sense.h"

class SenseLunarCycle : public Sense {
public:
    SenseLunarCycle(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner);
    virtual ~SenseLunarCycle() override;

    virtual std::string_view GetName() const override { return "SenseLunarCycle"; }

    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseParameters& universeParameters) const override;
};

#endif // SENSELUNARCYCLE_H
