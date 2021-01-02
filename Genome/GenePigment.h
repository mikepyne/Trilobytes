#ifndef GENEPIGMENT_H
#define GENEPIGMENT_H

#include "Gene.h"

class GenePigment : public Gene {
public:
    GenePigment();
    GenePigment(double a, double r, double g, double b, double dominance);
    virtual ~GenePigment() override;

protected:
    virtual std::shared_ptr<Gene> Copy() const override;
    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

private:
    double a_;
    double r_;
    double g_;
    double b_;
};

#endif // GENEPIGMENT_H
