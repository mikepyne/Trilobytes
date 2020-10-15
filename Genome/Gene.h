#ifndef GENE_H
#define GENE_H

#include <memory>

class Swimmer;
struct Phenotype;

class Gene {
public:
    Gene(double dominance, double mutationProbability);
    virtual ~Gene();

    static std::shared_ptr<Gene>& GetMostDominant(std::pair<std::shared_ptr<Gene>, std::shared_ptr<Gene>>& alleles);
    static const std::shared_ptr<Gene>& GetMostDominant(const std::pair<std::shared_ptr<Gene>, std::shared_ptr<Gene> >& alleles);
    static std::shared_ptr<Gene>& GetRandom(std::pair<std::shared_ptr<Gene>, std::shared_ptr<Gene>>& alleles);
    static bool Empty(const std::pair<std::shared_ptr<Gene>, std::shared_ptr<Gene>>& alleles) { return !(alleles.first || alleles.second); };

    // For getting a copy plus mutation, no point copying a Gene if there's no
    // change to it, when they're all shared_ptrs
    virtual std::shared_ptr<Gene> Mutate() const = 0;

    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const = 0;

    // TODO (maybe...) Splice (implement a function that will create a new Gene by splicing two parent genes together)
    double GetDominance() const { return dominance_; }
    double GetMutationProbability() const { return mutationProbability_; }
    double GetMutatedDominance() const;
    double GetMutatedMutationProbability() const;

private:
    double dominance_;
    double mutationProbability_;
};

#endif // GENE_H
