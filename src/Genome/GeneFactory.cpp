#include "GeneFactory.h"

#include "GeneBrain.h"
#include "GenePigment.h"
#include "GeneSenseRandom.h"
#include "GeneSenseSine.h"
#include "GeneEffectorTail.h"
#include "GeneEffectorSpringTail.h"
#include "GeneEffectorFilterMouth.h"
#include "GeneEffectorProboscisMouth.h"
#include "GeneEffectorSpike.h"
#include "GeneSenseMagneticField.h"
#include "GeneSenseLunarCycle.h"
#include "GeneSenseTraitsInArea.h"
#include "GeneSenseTraitsTouching.h"
#include "GeneSenseTraitsRaycast.h"
#include "GeneSenseTraitsSelf.h"
#include "Utility/Random.h"

const GeneFactory& GeneFactory::Get()
{
    static const GeneFactory factory;
    return factory;
}

std::shared_ptr<Genome> GeneFactory::GenerateCustomGenome(std::map<std::string, unsigned> geneCounts, unsigned brainWidth) const
{
    std::vector<std::shared_ptr<Gene>> genes;
    for (const auto& [ geneName, count ] : geneCounts) {
        for (unsigned i = 0; i < count; ++i) {
            genes.push_back(allGeneGenerators_.at(geneName)(brainWidth));
        }
    }
    return std::make_shared<Genome>(std::move(genes));
}

std::shared_ptr<Genome> GeneFactory::GenerateCustomGenome(std::map<Tag, unsigned> geneCounts, unsigned brainWidth) const
{
    std::vector<std::shared_ptr<Gene>> genes;
    for (const auto& [ tag, count ] : geneCounts) {
        Random::ForNItems(groupedGeneGenerators_.at(tag), count, [&](const GeneratorType& generator)
        {
            genes.push_back(generator(brainWidth));
        });
    }
    return std::make_shared<Genome>(std::move(genes));
}

std::shared_ptr<Genome> GeneFactory::GenerateDefaultGenome(unsigned brainWidth) const
{
    // FIXME ought to just have a directory of commited
    if (Random::Boolean()) {
        return GenerateCustomGenome({
                                        { GenePigment::Name(), 2 },
                                        { GeneBrain::Name(), 1 },
                                        { GeneSenseRandom::Name(), 1 },
                                        { GeneSenseMagneticField::Name(), 1 },
                                        { GeneSenseTraitsInArea::Name(), 2 },
                                        { GeneSenseTraitsRaycast::Name(), 1 },
                                        { GeneSenseTraitsTouching::Name(), 1 },
                                        { GeneSenseTraitsSelf::Name(), 1 },
                                        { GeneEffectorTail::Name(), 1 },
                                        { GeneEffectorFilterMouth::Name(), 1 },
                                    }, brainWidth);
    } else {
        return std::make_shared<Genome>(std::vector<std::shared_ptr<Gene>>{
            std::make_shared<GenePigment>(),
            std::make_shared<GenePigment>(),
            std::make_shared<GeneBrain>(3u, NeuralNetwork::BRAIN_WIDTH),
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
            std::make_shared<GeneSenseMagneticField>(0, NeuralNetwork::BRAIN_WIDTH, Point{Random::Number(-1000.0, 1000.0), Random::Number(-1000.0, 1000.0)}),
            std::make_shared<GeneEffectorFilterMouth>(0, NeuralNetwork::BRAIN_WIDTH),
        });
    }
}

std::shared_ptr<Genome> GeneFactory::GenerateRandomGenome(unsigned brainWidth) const
{
    return GenerateCustomGenome({
                                    { Tag::Brain, 1 },
                                    { Tag::Sense, 0 },
                                    { Tag::SenseTraits, 4 },
                                    { Tag::SenseEnvironment, 1 },
                                    { Tag::Effector, 0 },
                                    { Tag::FeedingApparatus, 1 },
                                    { Tag::Locomotion, 1 },
                                    { Tag::Appearance, 3 },
                                    { Tag::Weapon, Random::PercentChance(1.0) ? 1 : 0 },
                                }, brainWidth);
}

std::shared_ptr<Gene> GeneFactory::GenerateRandomGene(unsigned brainWidth) const
{
    return Random::Item(allGeneGenerators_).second(brainWidth);
}

std::shared_ptr<Gene> GeneFactory::GenerateRandomGene(GeneFactory::Tag type, unsigned brainWidth) const
{
    return Random::Item(groupedGeneGenerators_.at(type))(brainWidth);
}

GeneFactory::GeneFactory()
{
    RegisterGeneType<GeneBrain>({ Tag::Brain });

    RegisterGeneType<GeneSenseTraitsTouching>({ Tag::Sense, Tag::SenseTraits });
    RegisterGeneType<GeneSenseTraitsInArea>({ Tag::Sense, Tag::SenseTraits });
    RegisterGeneType<GeneSenseTraitsRaycast>({ Tag::Sense, Tag::SenseTraits });
    RegisterGeneType<GeneSenseTraitsSelf>({ Tag::Sense, Tag::SenseTraits, Tag::Homeostasis });

    RegisterGeneType<GeneSenseMagneticField>({ Tag::Sense, Tag::SenseEnvironment });
    RegisterGeneType<GeneSenseRandom>({ Tag::Sense, Tag::SenseEnvironment });
    RegisterGeneType<GeneSenseSine>({ Tag::Sense, Tag::SenseEnvironment });
    RegisterGeneType<GeneSenseLunarCycle>({ Tag::Sense, Tag::SenseEnvironment });

    RegisterGeneType<GeneEffectorTail>({ Tag::Effector, Tag::Locomotion });
    RegisterGeneType<GeneEffectorSpringTail>({ Tag::Effector, Tag::Locomotion });

    RegisterGeneType<GeneEffectorFilterMouth>({ Tag::Effector, Tag::FeedingApparatus });
    RegisterGeneType<GeneEffectorProboscisMouth>({ Tag::Effector, Tag::FeedingApparatus });

    RegisterGeneType<GeneEffectorSpike>({ Tag::Weapon });

    RegisterGeneType<GenePigment>({ Tag::Appearance });
}
