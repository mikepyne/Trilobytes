#ifndef CHROMOSOMEPAIR_H
#define CHROMOSOMEPAIR_H

#include "Gene.h"
#include "UniverseParameters.h"
#include "Utility/Range.h"

#include <map>
#include <vector>
#include <memory>
#include <optional>
#include <functional>

class ChromosomePair {
public:
    // Create two identical chromosomes
    ChromosomePair(std::vector<std::shared_ptr<Gene>>&& genes);

    static nlohmann::json Serialise(const ChromosomePair& genome);
    // static std::optional<ChromosomePair> Deserialise(const nlohmann::json& serialisedGenome);

    void ForEach(const std::function<void(const Gene& gene)>& action) const;

    static std::optional<ChromosomePair> Recombine(const ChromosomePair& maternal, const ChromosomePair& paternal);

    void MutateGene();
    void MutateStructure();

private:
    static const inline std::string KEY_A_CHROMOSOME_RANGE = "ARange";
    static const inline std::string KEY_B_CHROMOSOME_RANGE = "BRange";
    static const inline std::string KEY_ALLELES = "Alleles";
    static const inline std::string KEY_ALLELE_LOCUS = "Locus";
    static const inline std::string KEY_ALLELE_GENE_A_NAME = "NameA";
    static const inline std::string KEY_ALLELE_GENE_A = "GeneA";
    static const inline std::string KEY_ALLELE_GENE_B_NAME = "NameB";
    static const inline std::string KEY_ALLELE_GENE_B = "GeneB";

    Tril::Range<unsigned> aChromosomeRange_;
    Tril::Range<unsigned> bChromosomeRange_;
    std::map<unsigned, std::pair<std::shared_ptr<Gene>, std::shared_ptr<Gene>>> chromosomePair_;

    ChromosomePair(const Tril::Range<unsigned>& aRange, const Tril::Range<unsigned>& bRange, std::map<unsigned, std::pair<std::shared_ptr<Gene>, std::shared_ptr<Gene>>>&& chromosomePair);
};

#endif // CHROMOSOMEPAIR_H
