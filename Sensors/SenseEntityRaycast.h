#ifndef SENSEENTITYRAYCAST_H
#define SENSEENTITYRAYCAST_H

#include "Sense.h"
#include "Entity.h"
#include "EntityContainerInterface.h"

class SenseEntityRaycast : public Sense {
public:
    SenseEntityRaycast(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, double maxDistance, double angle, double genericWeight, const std::vector<std::pair<double, Trait>>&& traits);

    virtual std::string_view GetName() const override { return "SenseEntityRaycast"; }
    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities) const override;

    virtual void Draw(QPainter& paint) const override;

private:
    double rayCastDistance_;
    double rayCastAngle_;
    double genericWeight_;
    std::vector<std::pair<double, Trait>> traits_;

    Line GetLine() const;
};

#endif // SENSEENTITYRAYCAST_H
