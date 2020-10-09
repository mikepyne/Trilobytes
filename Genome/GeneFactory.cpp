#include "GeneFactory.h"

#include "Random.h"
#include "GenePigment.h"
#include "GeneBrain.h"
#include "GeneSenseEntitiesInArea.h"
#include "GeneSenseEntitiesTouching.h"
#include "GeneSenseRandom.h"
#include "GeneSenseMagneticField.h"
#include "GeneSenseEntityRaycast.h"

std::shared_ptr<Gene> GeneFactory::Random(unsigned brainWidth)
{
    switch (Random::Number(0, 6)) {
    case 0 :
        return std::make_shared<GenePigment>();
    case 1 :
        return std::make_shared<GeneBrain>(Random::Number(1, 5), brainWidth, Random::Number(0.0, 1.0));
    case 2 : {
        std::vector<SenseTraitsBase::TraitNormaliser> traits;
        // Between 1 - 4 traits
        for (const SenseTraitsBase::Trait& trait : Random::CopyItems(Random::Number<size_t>(1, 4), SenseTraitsBase::ALL_TRAITS)) {
            traits.push_back(SenseTraitsBase::DefaultNormalisation(trait));
        }
        unsigned hiddenLayers = Random::Number(0u, traits.size());
        Transform transform = { 0, Random::Number(0.0, 20.0), Random::Gaussian(0.0, EoBE::Pi) };
        double radius = Random::Number(5.0, 30.0);
        return std::make_shared<GeneSenseEntitiesInArea>(std::move(traits), hiddenLayers, brainWidth, transform, radius);
    }
    case 3 :
        return std::make_shared<GeneSenseRandom>(Random::Number(1, 3), brainWidth);
    case 4 :
        return std::make_shared<GeneSenseMagneticField>(brainWidth);
    case 5 : {
        std::vector<SenseTraitsBase::TraitNormaliser> traits;
        // Between 1 - 4 traits
        for (const SenseTraitsBase::Trait& trait : Random::CopyItems(Random::Number<size_t>(1, 4), SenseTraitsBase::ALL_TRAITS)) {
            traits.push_back(SenseTraitsBase::DefaultNormalisation(trait));
        }
        unsigned hiddenLayers = Random::Number(0u, traits.size());
        Transform transform = { 0, Random::Number(0.0, 20.0), Random::Gaussian(0.0, EoBE::Pi) };
        return std::make_shared<GeneSenseEntitiesTouching>(std::move(traits), hiddenLayers, brainWidth, transform);
    }
    case 6 : {
        std::vector<SenseTraitsBase::TraitNormaliser> traits;
        // Between 1 - 4 traits
        for (const SenseTraitsBase::Trait& trait : Random::CopyItems(Random::Number<size_t>(1, 4), SenseTraitsBase::ALL_TRAITS)) {
            traits.push_back(SenseTraitsBase::DefaultNormalisation(trait));
        }
        unsigned hiddenLayers = Random::Number(0u, traits.size());
        Transform transform = { 0, Random::Number(0.0, 20.0), Random::Gaussian(0.0, EoBE::Pi) };
        return std::make_shared<GeneSenseEntityRaycast>(std::move(traits), hiddenLayers, brainWidth, transform);
    }
    }
    return nullptr;
}
