#ifndef SENSETRAITSBASE_H
#define SENSETRAITSBASE_H

#include "Sense.h"
#include "Utils.h"
#include "Energy.h"
#include "Transform.h"

#include <functional>

class Entity;

class SenseTraitsBase : public Sense {
public:
    enum class Trait {
        Red,
        Green,
        Blue,
        Energy,
        Age,
        Size,
        Distance,
    };

    static constexpr std::array ALL_TRAITS = { Trait::Red, Trait::Green, Trait::Blue, Trait::Energy, Trait::Age, Trait::Size, Trait::Distance };

    /**
     * Used to convert the combined values of all detected traits into a value
     * between 0.0 -> 1.0. This allows senses to evolve their own sensitivities
     * to various traits, and to decide which traits they sense.
     *
     * TODO consider allowing non-linear normalisation options
     */
    struct TraitNormaliser {
        Trait trait;
        EoBE::Range<double> range;
    };

    /**
     * Chooses a reasonable range for each trait, such that it is likely that
     * the value will be normalised to a number between 0.0 -> 1.0 for a single
     * entity being detected at full strength.
     */
    static TraitNormaliser DefaultNormalisation(const Trait& trait)
    {
        switch (trait) {
        case Trait::Red :
            return { trait, { 0, 0.9 } };
        case Trait::Green :
            return { trait, { 0, 0.9 } };
        case Trait::Blue :
            return { trait, { 0, 0.9 } };
        case Trait::Energy :
            return { trait, { 0_j, 500_j } };
        case Trait::Age :
            return { trait, { 0, 50'000 } };
        case Trait::Size :
            return { trait, { 0, 30 } };
        case Trait::Distance :
            return { trait, { 0, 100 } };
        }
        assert(false && "Unimplemented Trait");
        return { static_cast<Trait>(-1), { 0, 0 }};
    }

    SenseTraitsBase(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, const Transform& transform, std::vector<TraitNormaliser>&& toDetect);

    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities) const override final;

protected:
    const Transform transform_;

private:
    const std::vector<TraitNormaliser> toDetect_;

    /**
     * This allows senses to respond to any trait easily, it is up to the caller
     * to normalise the value.
     */
    double GetTraitFrom(const Entity& target, Trait trait) const;

    virtual void FilterEntities(const EntityContainerInterface& entities, const std::function<void(const Entity& e, const double& scale)>& forEachEntity) const = 0;
};

#endif // SENSETRAITSBASE_H
