#ifndef GENEFACTORY_H
#define GENEFACTORY_H

#include "Genome.h"
#include "Gene.h"

#include <memory>
#include <vector>
#include <functional>
#include <type_traits>

class GeneFactory {
public:
    enum class Tag {
        Brain,
        Sense,
        SenseTraits,
        SenseEnvironment,
        Effector,
        Homeostasis,
        FeedingApparatus,
        Locomotion,
        Appearance,
        Weapon,
    };

    static const GeneFactory& Get();

    /**
     * Takes the string names of genes, and the number of genes to add.
     *
     * TODO allow loading of a genome specification JSON file that specify
     * string number pairs, in this case Gene name and gene count.
     */
    std::shared_ptr<Genome> GenerateCustomGenome(std::map<std::string, unsigned> geneCounts, unsigned brainWidth) const;
    /**
     * As each tag can refer to a number of different genes, this allows for
     * generation of varied genomes but without worrying about most individuals
     * being born lacking a brain, or any means to move or eat etc
     */
    std::shared_ptr<Genome> GenerateCustomGenome(std::map<Tag, unsigned> geneCounts, unsigned brainWidth) const;
    std::shared_ptr<Genome> GenerateDefaultGenome(unsigned brainWidth) const;
    std::shared_ptr<Genome> GenerateRandomGenome(unsigned brainWidth) const;
    std::shared_ptr<Gene> GenerateRandomGene(unsigned brainWidth) const;
    std::shared_ptr<Gene> GenerateRandomGene(Tag type, unsigned brainWidth) const;

private:
    using GeneratorType = std::function<std::shared_ptr<Gene>(unsigned brainWidth)>;

    std::map<std::string, GeneratorType> allGeneGenerators_;
    std::map<Tag, std::vector<GeneratorType>> groupedGeneGenerators_;

    GeneFactory();

    /**
     * This template is intended to make adding new Gene types less error prone.
     */
    template <typename GeneType>
    void RegisterGeneType(std::vector<Tag>&& tags)
    {
        static_assert(std::is_base_of_v<Gene, GeneType>, "GeneType must be child of Gene");
        static_assert(!std::is_same_v<Gene, GeneType>, "GeneType must not be of type Gene");
        static_assert(!std::is_abstract_v<GeneType>, "GeneType must not be abstract");
        // Allow variable Generate function signature, because not all genes require knowledge of brain width
        // but some absolutely cannot function without it
        static_assert(std::is_same_v<decltype(&GeneType::Generate), std::shared_ptr<Gene>(*)()>
                      || std::is_same_v<decltype(&GeneType::Generate), std::shared_ptr<Gene>(*)(unsigned)>,
                      "GeneType must implement  a static 'Generate' function returning a 'shared_ptr<Gene>' taking either no argument or 'unsigned brainWidth' only.");
        static_assert(std::is_same_v<decltype(&GeneType::Deserialise), std::shared_ptr<Gene>(*)(const nlohmann::json&)>,
                      "GeneType must implement  a static 'Deserialise' function returning a 'shared_ptr<Gene>' from a JSON object.");
        static_assert(std::is_same_v<decltype(&GeneType::Name), std::string(*)()>,
                      "GeneType must implement  a static 'Name' function returning the same value as the member function.");

        constexpr bool signatureRequiresBrainWidth = std::is_same_v<decltype(&GeneType::Generate), std::shared_ptr<Gene>(*)(unsigned)>;

        allGeneGenerators_.insert({ GeneType::Name(), [](unsigned brainWidth) {
            if constexpr (signatureRequiresBrainWidth) {
                return GeneType::Generate(brainWidth);
            } else {
                (void) brainWidth; // unused
                return GeneType::Generate();
            }
        } });

        for (Tag tag : tags) {
            groupedGeneGenerators_[tag].push_back([](unsigned brainWidth) {
                if constexpr (signatureRequiresBrainWidth) {
                    return GeneType::Generate(brainWidth);
                } else {
                    (void) brainWidth; // unused
                    return GeneType::Generate();
                }
            });
        }
    }
};

#endif // GENEFACTORY_H
