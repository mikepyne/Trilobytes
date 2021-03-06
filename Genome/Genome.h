#ifndef GENOME_H
#define GENOME_H

#include "Gene.h"
#include "Phenotype.h"
#include "ChromosomePair.h"
#include "UniverseParameters.h"

#include <vector>
#include <memory>
#include <functional>
#include <string>

class Genome {
public:
    // Create a single Chromosome pair with the following genes
    explicit Genome(std::vector<std::shared_ptr<Gene>>&& genes);
    Genome(std::vector<ChromosomePair>&& chromosomes, uint64_t geneMutationCount, uint64_t chromosomeMutationCount);

    static std::shared_ptr<Genome> CreateOffspring(const Genome& aGenome, const Genome& bGenome, const UniverseParameters& universeParameters);
    static nlohmann::json Serialise(const std::shared_ptr<Genome>& genome);
    // static std::shared_ptr<Genome> Deserialise(const nlohmann::json& serialisedGenome);

    Phenotype GetPhenoType(Swimmer& owner) const;

    uint64_t GetGeneMutationCount() const { return geneMutationCount_; }
    uint64_t GetChromosomeMutationCount() const { return chromosomeMutationCount_; }

    void Mutate(const UniverseParameters& universeParameters);

private:
    static const inline std::string KEY_GENE_MUTATION_COUNT = "GeneMutations";
    static const inline std::string KEY_CHROMOSOME_MUTATION_COUNT = "ChromosomeMutations";
    static const inline std::string KEY_CHROMOSOMES = "Chromosomes";

    // accumulate thise each generation
    uint64_t geneMutationCount_;
    uint64_t chromosomeMutationCount_;

    std::vector<ChromosomePair> chromosomes_;

    void ForEach(const std::function<void(const Gene& gene)>& action) const;
};

#endif // GENOME_H
