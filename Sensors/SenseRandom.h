#ifndef SENSERANDOM_H
#define SENSERANDOM_H

#include "Sense.h"

class SenseRandom : public Sense {
public:
    SenseRandom(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner);

    virtual std::string_view GetName() const override { return "Random"; }
    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseInfoStructRef& environment) const override;
};

#endif // SENSERANDOM_H
