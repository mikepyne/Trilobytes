#ifndef SENSEENTITIESTOUCHING_H
#define SENSEENTITIESTOUCHING_H

#include "Sense.h"

#include "Entity.h"

class QPainter;

class SenseEntitiesTouching : public Sense {
    public:
        SenseEntitiesTouching(Entity& owner, double offsetDistance, double offsetAngle, double genericDetectionWeight, const std::vector<std::pair<double, Trait>>&& toDetect);

        virtual std::string_view GetName() const override { return "SenseEntitiesTouching"; }
        virtual void Draw(QPainter& paint) const override;

    private:
        double offsetDistance_;
        double offsetAngle_;
        double genericDetectionWeight_;
        std::vector<std::pair<double, Trait>> toDetect_;

        virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseInfoStructRef& environment) override;
};

#endif // SENSEENTITIESTOUCHING_H
