#ifndef GENOME_H
#define GENOME_H

#include "Gene.h"
#include "ChromosomePair.h"

#include <vector>
#include <memory>
#include <functional>

// TODO Genome scale mutations (chromosome duplications, polyploidy, chromosome fusing and fragmenting)
class Genome {
public:
    // Create a single Chromosome pair with the following genes
    Genome(std::vector<std::shared_ptr<Gene>>&& genes);
    Genome(std::vector<ChromosomePair>&& chromosomes);

    static std::shared_ptr<Genome> CreateGenome(const Genome& aGenome, const Genome& bGenome);

    void ForEach(const std::function<void(const Gene& gene)>& action) const;

private:
    std::vector<ChromosomePair> chromosomes_;
};

#endif // GENOME_H
