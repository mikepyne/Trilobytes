#ifndef GENEDIGESTIVEENZYME_H
#define GENEDIGESTIVEENZYME_H

#include "Gene.h"
#include "Stomach.h"
#include "Random.h"

#include "Phenotype.h"

class GeneDigestiveEnzyme : public Gene {
public:
    GeneDigestiveEnzyme();
    GeneDigestiveEnzyme(DietType digestiveTarget, double dominance, double mutationProbability);

    virtual std::shared_ptr<Gene> Mutate() const override;
    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

private:
    DietType digestiveTarget_;
};

#endif // GENEDIGESTIVEENZYME_H
