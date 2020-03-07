#ifndef SENSERANDOM_H
#define SENSERANDOM_H

#include "Sense.h"

class SenseRandom : public Sense {
public:
    SenseRandom(Entity& owner, unsigned inputCount);
    virtual std::string_view GetName() const override { return "Random"; }

private:
    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseInfoStructRef& environment) override;
};

#endif // SENSERANDOM_H
