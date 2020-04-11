#include "Genome.h"

Genome::Genome(std::vector<std::shared_ptr<Gene> >&& genes)
    : chromosomes_({ ChromosomePair(std::move(genes)), })
{
}

Genome::Genome(std::vector<ChromosomePair>&& chromosomes)
    : chromosomes_(std::move(chromosomes))
{
}

std::shared_ptr<Genome> Genome::CreateGenome(const Genome& aGenome, const Genome& bGenome)
{
    if (aGenome.chromosomes_.size() == bGenome.chromosomes_.size()) {
        std::vector<ChromosomePair> newChromosomes;

        auto aIter = aGenome.chromosomes_.cbegin();
        auto bIter = bGenome.chromosomes_.cbegin();
        for (; aIter != aGenome.chromosomes_.cend() ; ++aIter, ++bIter) {
            if (std::optional<ChromosomePair> crossed = ChromosomePair::Recombine(*aIter, *bIter)) {
                newChromosomes.push_back(std::move(crossed.value()));
                continue;
            }
            return {};
        }
        return std::make_shared<Genome>(std::move(newChromosomes));
    }
    return {};
}

void Genome::ForEach(const std::function<void (const Gene&)>& action) const {
    for (const ChromosomePair& chromosome : chromosomes_) {
        chromosome.ForEach(action);
    }
}
