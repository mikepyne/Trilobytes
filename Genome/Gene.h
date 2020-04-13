#ifndef GENE_H
#define GENE_H

#include <memory>

class Gene {
public:
    Gene(double dominance);
    virtual ~Gene();

    static const std::shared_ptr<Gene>& GetMostDominant(const std::shared_ptr<Gene>& a, const std::shared_ptr<Gene>& b);

    // For getting a copy plus mutation, no point copying a Gene if there's no
    // change to it, when they're all shared_ptrs
    virtual std::shared_ptr<Gene> Mutate() const = 0;
    // TODO Splice (implement a function that will create a new Gene by splicing two parent genes together)

private:
    double dominance_;
};

#endif // GENE_H
