#include "GeneFactory.h"

#include "Random.h"
#include "GenePigment.h"
#include "GeneBrain.h"
#include "GeneSenseEntitiesInArea.h"
#include "GeneSenseEntitiesTouching.h"
#include "GeneSenseRandom.h"
#include "GeneSenseMagneticField.h"

std::shared_ptr<Gene> GeneFactory::Random(unsigned brainWidth)
{
    switch (Random::Number(0, 5)) {
    case 0 :
        return std::make_shared<GenePigment>();
    case 1 :
        return std::make_shared<GeneBrain>(Random::Number(1, 5), brainWidth, Random::Number(0.0, 1.0));
    case 2 : {
        std::vector<std::pair<double, Trait>> additionalTraits;
        for (auto& trait : ALL_TRAITS) {
            if (Random::Percent() <= 5.0) {
                additionalTraits.emplace_back(Random::Gaussian(1.0, 0.5), trait);
            }
        }
        unsigned hiddenLayers = Random::Number(0u, additionalTraits.size());
        Transform transform = { 0, Random::Number(0.0, 20.0), Random::Gaussian(0.0, EoBE::Pi) };
        double radius = Random::Number(5.0, 30.0);
        return std::make_shared<GeneSenseEntitiesInArea>(std::move(additionalTraits), hiddenLayers, brainWidth, transform, radius, 1.0);
    }
    case 3 :
        return std::make_shared<GeneSenseRandom>(Random::Number(1, 3), brainWidth);
    case 4 :
        return std::make_shared<GeneSenseMagneticField>(brainWidth);
    case 5 : {
        std::vector<std::pair<double, Trait>> additionalTraits;
        for (auto& trait : ALL_TRAITS) {
            if (Random::Percent() <= 5.0) {
                additionalTraits.emplace_back(Random::Gaussian(1.0, 0.5), trait);
            }
        }
        unsigned hiddenLayers = Random::Number(0u, additionalTraits.size());
        Transform transform = { 0, Random::Number(0.0, 20.0), Random::Gaussian(0.0, EoBE::Pi) };
        return std::make_shared<GeneSenseEntitiesTouching>(std::move(additionalTraits), hiddenLayers, brainWidth, transform, 1.0);
    }
    }
    return nullptr;
}
