#include "Gene.h"

#include "Random.h"

Gene::Gene(double dominance, double mutationProbability)
    : dominance_(dominance)
    , mutationProbability_(mutationProbability)
{
}

Gene::~Gene()
{
}

std::shared_ptr<Gene>& Gene::GetMostDominant(std::pair<std::shared_ptr<Gene>, std::shared_ptr<Gene> >& alleles)
{
    if (alleles.first == nullptr) {
        return alleles.second;
    } else if (alleles.second == nullptr) {
        return alleles.first;
    } else {
        return alleles.first->dominance_ > alleles.second->dominance_ ? alleles.first : alleles.second;
    }
}

const std::shared_ptr<Gene>& Gene::GetMostDominant(const std::pair<std::shared_ptr<Gene>, std::shared_ptr<Gene> >& alleles)
{
    if (alleles.first == nullptr) {
        return alleles.second;
    } else if (alleles.second == nullptr) {
        return alleles.first;
    } else {
        return alleles.first->dominance_ > alleles.second->dominance_ ? alleles.first : alleles.second;
    }
}

std::shared_ptr<Gene>& Gene::GetRandom(std::pair<std::shared_ptr<Gene>, std::shared_ptr<Gene> >& alleles)
{
    return Random::Boolean() ? alleles.first : alleles.second;
}

double Gene::GetMutatedDominance() const
{
    return dominance_ + Random::Gaussian(0.0, 5.0);
}

double Gene::GetMutatedMutationProbability() const
{
    return mutationProbability_ + Random::Gaussian(0.0, 0.05);
}
