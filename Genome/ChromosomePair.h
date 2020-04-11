#ifndef CHROMOSOMEPAIR_H
#define CHROMOSOMEPAIR_H

#include "Utils.h"
#include "Gene.h"

#include <map>
#include <vector>
#include <memory>
#include <optional>
#include <functional>

// TODO chromosome scale mutations (chromosome length changes, inversions, gene duplication/loss)
class ChromosomePair {
public:
    // Create two identical chromosomes
    ChromosomePair(std::vector<std::shared_ptr<Gene>>&& genes);

    static std::optional<ChromosomePair> Recombine(const ChromosomePair& maternal, const ChromosomePair& paternal);

    void ForEach(const std::function<void(const Gene& gene)>& action) const;

private:
    EoBE::Range<unsigned> aChromosomeRange_;
    EoBE::Range<unsigned> bChromosomeRange_;
    std::map<unsigned, std::pair<std::shared_ptr<Gene>, std::shared_ptr<Gene>>> chromosomePair_;

    ChromosomePair(const EoBE::Range<unsigned>& aRange, const EoBE::Range<unsigned>& bRange, std::map<unsigned, std::pair<std::shared_ptr<Gene>, std::shared_ptr<Gene>>>&& chromosomePair);
};

#endif // CHROMOSOMEPAIR_H
