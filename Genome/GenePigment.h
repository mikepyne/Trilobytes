#ifndef GENEPIGMENT_H
#define GENEPIGMENT_H

#include "Gene.h"

class GenePigment : public Gene {
public:
    GenePigment();
    GenePigment(double a, double r, double g, double b, double dominance, double mutationProbability);
    virtual ~GenePigment() override final;

protected:
    virtual std::shared_ptr<Gene> Mutate() const override;
    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

private:
    const double a_;
    const double r_;
    const double g_;
    const double b_;
};

#endif // GENEPIGMENT_H
