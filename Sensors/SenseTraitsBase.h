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
        Health,
    };

    static constexpr std::array ALL_TRAITS = { Trait::Red, Trait::Green, Trait::Blue, Trait::Energy, Trait::Age, Trait::Size, Trait::Distance, Trait::Health };

    static std::string ToString(Trait trait);

    /**
     * Used to convert the combined values of all detected traits into a value
     * between 0.0 -> 1.0. This allows senses to evolve their own sensitivities
     * to various traits, and to decide which traits they sense.
     *
     * TODO consider allowing non-linear normalisation options
     */
    struct TraitNormaliser {
        Trait trait;
        EoBE::RangeConverter range;
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
            return { trait, { EoBE::Range(0.0, 0.9), EoBE::Range(-1.0, 1.0) } };
        case Trait::Green :
            return { trait, { EoBE::Range(0.0, 0.9), EoBE::Range(-1.0, 1.0) } };
        case Trait::Blue :
            return { trait, { EoBE::Range(0.0, 0.9), EoBE::Range(-1.0, 1.0) } };
        case Trait::Energy :
            return { trait, { EoBE::Range(0_j, 500_j), EoBE::Range(-1.0, 1.0) } };
        case Trait::Age :
            return { trait, { EoBE::Range(0.0, 50'000.9), EoBE::Range(-1.0, 1.0) } };
        case Trait::Size :
            return { trait, { EoBE::Range(0.0, 30.0), EoBE::Range(-1.0, 1.0) } };
        case Trait::Distance :
            return { trait, { EoBE::Range(0.0, 100.0), EoBE::Range(-1.0, 1.0) } };
        case Trait::Health :
            return { trait, { EoBE::Range(0.0, 100.0), EoBE::Range(-1.0, 1.0) } };
        }
        assert(false && "Unimplemented Trait");
        return { static_cast<Trait>(-1), { EoBE::Range(0.0, 0.0), EoBE::Range(-1.0, 1.0) }};
    }

    SenseTraitsBase(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, const Transform& transform, std::vector<TraitNormaliser>&& toDetect);

    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseParameters& universeParameters) const override final;

    const std::vector<TraitNormaliser>& GetDetectableTraits() const { return toDetect_; }

protected:
    const Transform transform_;

private:
    const std::vector<TraitNormaliser> toDetect_;

    /**
     * This allows senses to respond to any trait easily, it is up to the caller
     * to normalise the value.
     */
    double GetTraitFrom(const Entity& target, Trait trait) const;

    virtual void FilterEntities(const EntityContainerInterface& entities, const std::function<void(const Entity& e)>& forEachEntity) const = 0;
};

#endif // SENSETRAITSBASE_H
