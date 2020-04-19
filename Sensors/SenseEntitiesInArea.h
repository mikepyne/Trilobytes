#ifndef SENSEENTITIESINAREA_H
#define SENSEENTITIESINAREA_H

#include "Sense.h"

#include "Entity.h"

class QPainter;

class SenseEntitiesInArea : public Sense {
public:
    SenseEntitiesInArea(Entity& owner, double maxDistance, double offsetDistance, double offsetAngle, double senseDistanceWeight, const std::vector<std::pair<double, Trait>>&& toDetect);

    virtual std::string_view GetName() const override { return "SenseEntitiesInArea"; }
    virtual void Draw(QPainter& paint) const override;

private:
    double senseRadius_;
    double offsetDistance_;
    double offsetAngle_;
    double senseDistanceWeight_;
    std::vector<std::pair<double, Trait>> toDetect_;

    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseInfoStructRef& environment) override;

    Circle GetArea() const;

};

#endif // SENSEENTITIESINAREA_H
