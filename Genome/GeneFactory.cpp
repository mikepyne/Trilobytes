#include "GeneFactory.h"

#include "Random.h"
#include "GeneBrain.h"
#include "GenePigment.h"
#include "GeneSenseRandom.h"
#include "GeneEffectorTail.h"
#include "GeneSenseMagneticField.h"
#include "GeneSenseTraitsInArea.h"
#include "GeneSenseTraitsTouching.h"
#include "GeneSenseTraitsRaycast.h"
#include "GeneSenseTraitsSelf.h"

#include <vector>

std::shared_ptr<Genome> GeneFactory::DefaultGenome()
{
    return std::make_shared<Genome>(std::vector<std::shared_ptr<Gene>>{
        std::make_shared<GenePigment>(),
        std::make_shared<GenePigment>(),
        std::make_shared<GeneBrain>(3, NeuralNetwork::BRAIN_WIDTH, 0.5),
        std::make_shared<GeneSenseRandom>(1, NeuralNetwork::BRAIN_WIDTH),
        std::make_shared<GeneSenseTraitsInArea>(std::vector{
                                                    SenseTraitsBase::DefaultNormalisation(SenseTraitsBase::Trait::Distance),
                                                    SenseTraitsBase::DefaultNormalisation(SenseTraitsBase::Trait::Green), },
                                                0, NeuralNetwork::BRAIN_WIDTH, Transform{ 0, 24,  0.6}, 20),
        std::make_shared<GeneSenseTraitsInArea>(std::vector{
                                                    SenseTraitsBase::DefaultNormalisation(SenseTraitsBase::Trait::Distance),
                                                    SenseTraitsBase::DefaultNormalisation(SenseTraitsBase::Trait::Green), },
                                                0, NeuralNetwork::BRAIN_WIDTH, Transform{ 0, 24, -0.6}, 20),
        std::make_shared<GeneSenseTraitsRaycast>(std::vector{
                                                     SenseTraitsBase::DefaultNormalisation(SenseTraitsBase::Trait::Distance),
                                                     SenseTraitsBase::DefaultNormalisation(SenseTraitsBase::Trait::Size), },
                                                 0, NeuralNetwork::BRAIN_WIDTH, Transform{ 0, 0, 0 }, 30, 0),
        std::make_shared<GeneSenseTraitsTouching>(std::vector{
                                                      SenseTraitsBase::DefaultNormalisation(SenseTraitsBase::Trait::Energy), },
                                                  0, NeuralNetwork::BRAIN_WIDTH, Transform{ 0, 7.5, 0 }),
        std::make_shared<GeneSenseTraitsSelf>(std::vector{
                                                  SenseTraitsBase::DefaultNormalisation(SenseTraitsBase::Trait::Age),
                                                  SenseTraitsBase::DefaultNormalisation(SenseTraitsBase::Trait::Energy), },
                                              0, NeuralNetwork::BRAIN_WIDTH),
        std::make_shared<GeneEffectorTail>(0, NeuralNetwork::BRAIN_WIDTH),
        std::make_shared<GeneSenseMagneticField>(0, NeuralNetwork::BRAIN_WIDTH),
                                    });
}

std::shared_ptr<Genome> GeneFactory::RandomGenome()
{
    std::vector<std::shared_ptr<Gene>> genes;
    unsigned numGenes = 20;
    for (unsigned i = 0; i < numGenes; ++i) {
        genes.push_back(RandomGene(NeuralNetwork::BRAIN_WIDTH));
    }
    return std::make_shared<Genome>(std::move(genes));
}

std::shared_ptr<Gene> GeneFactory::RandomGene(unsigned brainWidth)
{
    std::vector<std::function<std::shared_ptr<Gene>()>> genes{
    []()
    {
        return std::make_shared<GenePigment>();
    },
    [=]()
    {
        return std::make_shared<GeneBrain>(Random::Number(1, 5), brainWidth, Random::Number(0.0, 1.0));
    },
    [=]()
    {
        std::vector<SenseTraitsBase::TraitNormaliser> traits;
        // Between 1 - 4 traits
        for (const SenseTraitsBase::Trait& trait : Random::CopyItems(Random::Number<size_t>(1, 4), SenseTraitsBase::ALL_TRAITS)) {
            traits.push_back(SenseTraitsBase::DefaultNormalisation(trait));
        }
        unsigned hiddenLayers = Random::Number(0u, traits.size());
        Transform transform = { 0, Random::Number(0.0, 20.0), Random::Gaussian(0.0, EoBE::Pi) };
        double radius = Random::Number(5.0, 30.0);
        return std::make_shared<GeneSenseTraitsInArea>(std::move(traits), hiddenLayers, brainWidth, transform, radius);
    },
    [=]()
    {
        return std::make_shared<GeneSenseRandom>(Random::Number(1, 3), brainWidth);
    },
    [=]()
    {
        return std::make_shared<GeneSenseMagneticField>(Random::Number(0, 2), brainWidth);
    },
    [=]()
    {
        std::vector<SenseTraitsBase::TraitNormaliser> traits;
        // Between 1 - 4 traits
        for (const SenseTraitsBase::Trait& trait : Random::CopyItems(Random::Number<size_t>(1, 4), SenseTraitsBase::ALL_TRAITS)) {
            traits.push_back(SenseTraitsBase::DefaultNormalisation(trait));
        }
        unsigned hiddenLayers = Random::Number(0u, traits.size());
        Transform transform = { 0, Random::Number(0.0, 20.0), Random::Gaussian(0.0, EoBE::Pi) };
        return std::make_shared<GeneSenseTraitsTouching>(std::move(traits), hiddenLayers, brainWidth, transform);
    },
    [=]()
    {
        std::vector<SenseTraitsBase::TraitNormaliser> traits;
        // Between 1 - 4 traits
        for (const SenseTraitsBase::Trait& trait : Random::CopyItems(Random::Number<size_t>(1, 4), SenseTraitsBase::ALL_TRAITS)) {
            traits.push_back(SenseTraitsBase::DefaultNormalisation(trait));
        }
        unsigned hiddenLayers = Random::Number(0u, traits.size());
        double distance = Random::Number(1.0, 50.0);
        double rotation = Random::Number(0.0, EoBE::Tau);
        // TODO allow the sense to start somewhere other than the centre of the swimmer
        return std::make_shared<GeneSenseTraitsRaycast>(std::move(traits), hiddenLayers, brainWidth, Transform{ 0, 0, 0 }, distance, rotation);
    },
    [=]()
    {
        std::vector<SenseTraitsBase::TraitNormaliser> traits;
        // Between 1 - 4 traits
        for (const SenseTraitsBase::Trait& trait : Random::CopyItems(Random::Number<size_t>(1, 4), SenseTraitsBase::ALL_TRAITS)) {
            traits.push_back(SenseTraitsBase::DefaultNormalisation(trait));
        }
        unsigned hiddenLayers = Random::Number(0u, traits.size());
        return std::make_shared<GeneSenseTraitsSelf>(std::move(traits), hiddenLayers, brainWidth);
    },
    [=]()
    {
        unsigned hiddenLayers = Random::Number(0u, 2u);
        return std::make_shared<GeneEffectorTail>(hiddenLayers, brainWidth);
    },
    };
    return Random::Item(genes)();
}
