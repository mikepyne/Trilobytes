#ifndef SENSEENTITYRAYCAST_H
#define SENSEENTITYRAYCAST_H

#include "Sense.h"
#include "Entity.h"
#include "EntityContainerInterface.h"

class SenseEntityRaycast : public Sense {
public:
    SenseEntityRaycast(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, double maxDistance, double angle, const std::vector<Trait>&& toDetect);

    virtual std::string_view GetName() const override { return "SenseEntityRaycast"; }
    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities) const override;

    virtual void Draw(QPainter& paint) const override;

private:
    double rayCastDistance_;
    double rayCastAngle_;
    std::vector<Trait> toDetect_;

    Line GetLine() const;
};

#endif // SENSEENTITYRAYCAST_H
