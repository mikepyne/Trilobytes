#include "SenseRandom.h"

#include "Random.h"

SenseRandom::SenseRandom(Swimmer& owner, unsigned inputCount)
    : Sense(owner, inputCount, 0)
{
}

void SenseRandom::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& /*entities*/, const Sense::UniverseInfoStructRef& /*environment*/)
{
    auto rand = Random::Numbers(inputs.size(), -1.0, 1.0);
    inputs.swap(rand);
}
