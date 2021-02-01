#ifndef CHROMOSOMEPAIR_H
#define CHROMOSOMEPAIR_H

#include "Utils.h"
#include "Gene.h"
#include "UniverseParameters.h"

#include <map>
#include <vector>
#include <memory>
#include <optional>
#include <functional>

class ChromosomePair {
public:
    // Create two identical chromosomes
    ChromosomePair(std::vector<std::shared_ptr<Gene>>&& genes);

    void ForEach(const std::function<void(const Gene& gene)>& action) const;

    static std::optional<ChromosomePair> Recombine(const ChromosomePair& maternal, const ChromosomePair& paternal);

    void MutateGene();
    void MutateStructure();

private:
    Tril::Range<unsigned> aChromosomeRange_;
    Tril::Range<unsigned> bChromosomeRange_;
    std::map<unsigned, std::pair<std::shared_ptr<Gene>, std::shared_ptr<Gene>>> chromosomePair_;

    ChromosomePair(const Tril::Range<unsigned>& aRange, const Tril::Range<unsigned>& bRange, std::map<unsigned, std::pair<std::shared_ptr<Gene>, std::shared_ptr<Gene>>>&& chromosomePair);
};

#endif // CHROMOSOMEPAIR_H
