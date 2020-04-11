#include "Gene.h"

Gene::Gene(double dominance)
    : dominance_(dominance)
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
