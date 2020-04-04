#ifndef SENSEENTITYRAYCAST_H
#define SENSEENTITYRAYCAST_H

#include "Sense.h"
#include "Entity.h"
#include "EntityContainerInterface.h"

class SenseEntityRaycast : public Sense {
public:
    SenseEntityRaycast(Entity& owner, double maxDistance, double angle);

    virtual std::string_view GetName() const override { return "SenseEntityRaycast"; }
    virtual void Draw(QPainter& paint) const override;

private:
    double rayCastDistance_;
    double rayCastAngle_;

    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseInfoStructRef& /*environment*/) override;

    Line GetLine() const;
};

#endif // SENSEENTITYRAYCAST_H
