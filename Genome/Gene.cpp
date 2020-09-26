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

const std::shared_ptr<Gene>& Gene::GetMostDominant(const std::shared_ptr<Gene>& a, const std::shared_ptr<Gene>& b)
{
    if (a == nullptr) {
        return b;
    } else if (b == nullptr) {
        return a;
    } else {
        return  a->dominance_ > b->dominance_ ? a : b;
    }
}

double Gene::GetMutatedDominance() const
{
    return dominance_ + Random::Gaussian(0.0, 5.0);
}

double Gene::GetMutatedMutationProbability() const
{
    return mutationProbability_ + Random::Gaussian(0.0, 0.05);
}
