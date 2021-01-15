#include "GeneDigestiveEnzyme.h"


GeneDigestiveEnzyme::GeneDigestiveEnzyme()
    : GeneDigestiveEnzyme(Random::Item(std::array{ DietType::Carnivore, DietType::Herbivore, DietType::Eggivore}), Random::Number(0.0, 100.0), Random::Number(0.0, 0.2))
{
}

GeneDigestiveEnzyme::GeneDigestiveEnzyme(DietType digestiveTarget, double dominance, double mutationProbability)
    : Gene(dominance, mutationProbability)
    , digestiveTarget_(digestiveTarget)
{
}

std::shared_ptr<Gene> GeneDigestiveEnzyme::Mutate() const
{
    return std::make_shared<GeneDigestiveEnzyme>(Random::Item(std::array{ DietType::Carnivore, DietType::Herbivore, DietType::Eggivore}), GetMutatedDominance(), GetMutatedMutationProbability());
}

void GeneDigestiveEnzyme::ExpressGene(Swimmer& /*owner*/, Phenotype& target) const
{
    target.diet.AddEnzyme(digestiveTarget_);
}
