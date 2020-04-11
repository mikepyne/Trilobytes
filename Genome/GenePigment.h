#ifndef GENEPIGMENT_H
#define GENEPIGMENT_H

#include "Gene.h"
#include "Random.h"

class GenePigment : public Gene {
public:
    const double a_;
    const double r_;
    const double g_;
    const double b_;

    GenePigment();
    GenePigment(double a, double r, double g, double b);
    virtual ~GenePigment() override;

protected:
    virtual std::shared_ptr<Gene> Mutate() const override;
};

#endif // GENEPIGMENT_H
