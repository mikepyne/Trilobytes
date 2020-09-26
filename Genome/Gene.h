#ifndef GENE_H
#define GENE_H

#include <memory>

class Swimmer;
class Phenotype;

class Gene {
public:
    Gene(double dominance, double mutationProbability);
    virtual ~Gene();

    static const std::shared_ptr<Gene>& GetMostDominant(const std::shared_ptr<Gene>& a, const std::shared_ptr<Gene>& b);

    // For getting a copy plus mutation, no point copying a Gene if there's no
    // change to it, when they're all shared_ptrs
    virtual std::shared_ptr<Gene> Mutate() const = 0;

    virtual void ExpressGene(const Swimmer& owner, Phenotype& target) const = 0;

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
