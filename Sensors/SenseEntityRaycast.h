#ifndef SENSEENTITYRAYCAST_H
#define SENSEENTITYRAYCAST_H

#include "Sense.h"
#include "Entity.h"
#include "EntityContainerInterface.h"

class Swimmer;

class SenseEntityRaycast : public Sense {
public:
    SenseEntityRaycast(Swimmer& owner, double maxDistance, double angle, const std::vector<Trait>&& toDetect);

    virtual std::string_view GetName() const override { return "SenseEntityRaycast"; }
    virtual void Draw(QPainter& paint) const override;

private:
    double rayCastDistance_;
    double rayCastAngle_;
    std::vector<Trait> toDetect_;

    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseInfoStructRef& /*environment*/) override;

    Line GetLine() const;
};

#endif // SENSEENTITYRAYCAST_H
