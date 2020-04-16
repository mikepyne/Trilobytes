#include "GenePigment.h"

GenePigment::GenePigment()
    : GenePigment(Random::Number(-0.5, 0.5), Random::Number(-0.5, 0.5), Random::Number(-0.5, 0.5), Random::Number(-0.5, 0.5), 1.0 / 1000.0)
{
}

GenePigment::GenePigment(double a, double r, double g, double b, double mutationProbability)
    : Gene(Random::Number(0.0, 100.0), mutationProbability)
    , a_(std::clamp(a, -1.0, 1.0))
    , r_(std::clamp(r, -1.0, 1.0))
    , g_(std::clamp(g, -1.0, 1.0))
    , b_(std::clamp(b, -1.0, 1.0))
{
}

GenePigment::~GenePigment()
{
}

std::shared_ptr<Gene> GenePigment::Mutate() const
{
    return std::make_shared<GenePigment>(a_ + Random::Number(-1.0, 1.0),
                                         r_ + Random::Number(-1.0, 1.0),
                                         g_ + Random::Number(-1.0, 1.0),
                                         b_ + Random::Number(-1.0, 1.0),
                                         GetMutationProbability() + Random::Number(-0.01, 0.01));
}
