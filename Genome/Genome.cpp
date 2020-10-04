#include "Genome.h"

#include "Random.h"

Genome::Genome(std::vector<std::shared_ptr<Gene> >&& genes)
    : chromosomes_({ ChromosomePair(std::move(genes)), })
{
}

Genome::Genome(std::vector<ChromosomePair>&& chromosomes)
    : chromosomes_(std::move(chromosomes))
{
}

std::shared_ptr<Genome> Genome::CreateOffspring(const Genome& aGenome, const Genome& bGenome)
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
        auto offspring = std::make_shared<Genome>(std::move(newChromosomes));
        offspring->Mutate();
        if (offspring->chromosomes_.size() > 0) {
            return offspring;
        }
    }
    return {};
}

Phenotype Genome::GetPhenoType(const Swimmer& owner) const
{
    Phenotype phenotype;
    ForEach([&](const Gene& gene)
    {
        gene.ExpressGene(owner, phenotype);
    });
    return phenotype;
}

void Genome::ForEach(const std::function<void (const Gene&)>& action) const {
    for (const ChromosomePair& chromosome : chromosomes_) {
        chromosome.ForEach(action);
    }
}

void Genome::Mutate()
{
    // TODO allow the probabilities for various mutations to be controlled globally (perhaps allow genes that increase/reduce the rates of various mutations?)
    if (chromosomes_.size() > 0){
        // duplicate a chromosome
        if (!chromosomes_.empty() && Random::Number(0.0, 1.0) < (1.0 / 1000.0)) {
            size_t randomChromosome = Random::Number<size_t>(0, chromosomes_.size() - 1);
            chromosomes_.push_back(chromosomes_.at(randomChromosome));
        }

        // delete a chromosome
        if (!chromosomes_.empty() && Random::Number(0.0, 1.0) < (1.0 / 1000.0)) {
            int randomChromosome = Random::Number<int>(0, static_cast<int>(chromosomes_.size()) - 1);
            chromosomes_.erase(chromosomes_.begin() + Random::Number<int>(0, randomChromosome));
        }

        // duplicate all chromosomes
        if (Random::Number(0.0, 1.0) < (1.0 / 1000000.0)) {
            std::vector<ChromosomePair> copy = chromosomes_;
            std::move(std::begin(copy), std::end(copy), std::back_inserter(chromosomes_));
        }
    }

    // TODO chromosome fusing and fragmenting

    for (auto& chromosome : chromosomes_) {
        chromosome.Mutate();
    }

}
