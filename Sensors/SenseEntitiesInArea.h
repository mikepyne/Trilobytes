#ifndef SENSEENTITIESINAREA_H
#define SENSEENTITIESINAREA_H

#include "Sense.h"
#include "Shape.h"
#include "Transform.h"
#include "Entity.h"

class SenseEntitiesInArea : public Sense {
public:
    SenseEntitiesInArea(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, double maxDistance, const Transform& transform, double senseDistanceWeight, const std::vector<std::pair<double, Trait>>&& toDetect);

    virtual std::string_view GetName() const override { return "SenseEntitiesInArea"; }
    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseInfoStructRef& environment) const override;

    virtual void Draw(QPainter& paint) const override;

private:
    double senseRadius_;
    Transform transform_;
    double senseDistanceWeight_;
    std::vector<std::pair<double, Trait>> toDetect_;

    Circle GetArea() const;
};

#endif // SENSEENTITIESINAREA_H
