#include "Gene.h"

Gene::Gene(double dominance)
    : dominance_(dominance)
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

std::shared_ptr<Gene> Gene::Copy(unsigned mutationCount) const
{
    auto copy = Copy();
    copy->Mutate(mutationCount);
    return copy;
}

void Gene::Mutate(unsigned mutationCount)
{
    for (unsigned i = 0; i < mutationCount; ++i) {
        // pick from weighted distribution
        mutations_.RandomItem()();
        // Always mutate dominance when a mutation occurs
        dominance_ += Random::Gaussian(0.0, 5.0);
    }
}

void Gene::AddMutation(const double& probability, std::function<void ()>&& action)
{
    mutations_.PushBack(std::move(action), probability);
}
