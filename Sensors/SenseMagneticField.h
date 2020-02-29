#ifndef SENSEMAGNETICFIELD_H
#define SENSEMAGNETICFIELD_H

#include "Sense.h"

class SenseMagneticField : public Sense {
public:
    SenseMagneticField(Entity& owner);

    virtual std::string GetName() const override { return "MagneticField"; }
    virtual void Draw(QPainter& paint) const override;

private:
    double lastVector_;
    double lastDistance_;

    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& /*entities*/, const UniverseInfoStructRef& /*environment*/) override;
};

#endif // SENSEMAGNETICFIELD_H
