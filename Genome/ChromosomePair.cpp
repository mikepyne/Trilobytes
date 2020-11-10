#include "ChromosomePair.h"

#include "Random.h"
#include "GeneFactory.h"
#include "NeuralNetwork.h"

ChromosomePair::ChromosomePair(std::vector<std::shared_ptr<Gene> >&& genes)
    : aChromosomeRange_(Random::Number<unsigned>(), Random::Number<unsigned>())
    , bChromosomeRange_(aChromosomeRange_)
{
    for (auto& gene : genes) {
        chromosomePair_.insert({ Random::Number(aChromosomeRange_.Min(), aChromosomeRange_.Max()), std::make_pair(gene, gene) });
    }
}

std::optional<ChromosomePair> ChromosomePair::Recombine(const ChromosomePair& maternal, const ChromosomePair& paternal)
{
    // Take one chromosome from each pair
    bool takeFirstOfMaternal = Random::Boolean();
    bool takeFirstOfPaternal = Random::Boolean();

    EoBE::Range<unsigned> newAChromosomeRange_ = takeFirstOfMaternal ? maternal.aChromosomeRange_ : maternal.bChromosomeRange_;
    EoBE::Range<unsigned> newBChromosomeRange_ = takeFirstOfPaternal ? paternal.aChromosomeRange_ : paternal.bChromosomeRange_;

    // TODO consider how this prevents evolution of sexual dimorphism
    // Incompatible if chromosome ranges are too different
    if (newAChromosomeRange_.Similarity(newBChromosomeRange_) < Random::Number(0.95, 0.99)) {
        return {};
    }

    std::map<unsigned, std::pair<std::shared_ptr<Gene>, std::shared_ptr<Gene>>> newPair;
    double unpairedGeneCount = 0;

    // copy genes from one of the maternal chromosomes
    for (auto& [ location, genePair ] : maternal.chromosomePair_) {
        const std::shared_ptr<Gene>& geneToCopy = takeFirstOfMaternal ? genePair.first : genePair.second;
        if (geneToCopy) {
            newPair[location].first = geneToCopy;
            unpairedGeneCount++;
        }
    }
    // copy genes from one of the paternal chromosomes
    for (auto& [ location, genePair ] : paternal.chromosomePair_) {
        const std::shared_ptr<Gene>& geneToCopy = takeFirstOfPaternal ? genePair.first : genePair.second;
        if (geneToCopy) {
            newPair[location].second = geneToCopy;
            if (newPair[location].first != nullptr) {
                --unpairedGeneCount;
            } else {
                ++unpairedGeneCount;
            }
        }
    }

    // TODO consider how this prevents evolution of sexual dimorphism
    // Incompatible iff too many unpaired genes
    if (unpairedGeneCount / newPair.size() > Random::Number(0.01, 0.05)) {
        return {};
    }

    /*
     * Crossover Biologically, crossover occurs once sperm and egg have
     * fused, allowing grandchildren of the sperm and egg provider to
     * inherit genes from all four grandparents, despite only recieving one
     * chromosome from each pair from each parent.
     */
    // TODO crossover more likely at tips of chromosomes?
    if (size_t crossoverCount = Random::Poisson<size_t>(1); crossoverCount > 0) {
        std::vector<unsigned> crossoverLocations =  Random::Numbers(crossoverCount, std::min(newAChromosomeRange_.Min(), newBChromosomeRange_.Min()), std::max(newAChromosomeRange_.Max(), newBChromosomeRange_.Max()));
        bool swap = Random::Boolean();
        auto swapLocation = crossoverLocations.cbegin();
        // If started with swap, switch beginning of chromosome ranges
        if (swap) {
            auto aRangeFirst = newAChromosomeRange_.First();
            auto bRangeFirst = newBChromosomeRange_.First();
            newAChromosomeRange_.SetRange(bRangeFirst, newAChromosomeRange_.Last());
            newBChromosomeRange_.SetRange(aRangeFirst, newBChromosomeRange_.Last());
            // If even number of crossover locations we need to the ends of the ranges
            if (crossoverCount % 2 == 0) {
                auto aRangeFirst = newAChromosomeRange_.First();
                auto bRangeFirst = newBChromosomeRange_.First();
                newAChromosomeRange_.SetRange(bRangeFirst, newAChromosomeRange_.Last());
                newBChromosomeRange_.SetRange(aRangeFirst, newBChromosomeRange_.Last());
            }
        } else {
            // If odd number of crossover locations we need to the ends of the ranges
            if (crossoverCount % 2 != 0) {
                auto aRangeFirst = newAChromosomeRange_.First();
                auto bRangeFirst = newBChromosomeRange_.First();
                newAChromosomeRange_.SetRange(bRangeFirst, newAChromosomeRange_.Last());
                newBChromosomeRange_.SetRange(aRangeFirst, newBChromosomeRange_.Last());
            }
        }

        for (auto& [ location, genePair ] : newPair) {
            auto& [ a, b ] = genePair;
            while (swapLocation != crossoverLocations.cend() && *swapLocation <= location) {
                ++swapLocation;
                swap = !swap;
            }
            if (swap) {
                a.swap(b);
            }
        }
    }

    return { ChromosomePair(newAChromosomeRange_, newBChromosomeRange_, std::move(newPair)) };
}

void ChromosomePair::ForEach(const std::function<void (const Gene&)>& action) const
{
    for (auto& [ location, pair ] : chromosomePair_) {
        (void) location;
        if (!Gene::Empty(pair)) {
            action(*Gene::GetMostDominant(pair));
        }
    }
}

void ChromosomePair::Mutate()
{
    // MAYBE inversions

    // chromosome range change
    for (auto range : { &aChromosomeRange_, &bChromosomeRange_ }) {
        if (Random::Percent() < 0.5) {
            unsigned first = static_cast<unsigned>(Random::Gaussian<float>(0, range->ValueRange() / 200) + range->First());
            unsigned last = static_cast<unsigned>(Random::Gaussian<float>(0, range->ValueRange() / 200) + range->Last());
            range->SetRange(first, last);
        }
    }

    if (chromosomePair_.size() > 0) {
        // gene duplication
        if (Random::Percent() < 0.5) {
            auto& [ location, genePair ] = Random::Item(chromosomePair_);
            (void) location;
            std::shared_ptr<Gene>& randomGene = Gene::GetRandom(genePair);
            if (randomGene != nullptr) {
                bool addToA = Random::Boolean();
                size_t randomLocation = Random::Number<size_t>(addToA ? aChromosomeRange_.Min() : bChromosomeRange_.Min(), addToA ? aChromosomeRange_.Max() : bChromosomeRange_.Max());
                addToA ? chromosomePair_[randomLocation].first = randomGene : chromosomePair_[randomLocation].second = randomGene;
            }
        }

        // allele loss
        if (Random::Percent() < 0.5) {
            auto& [ location, genePair ] = Random::Item(chromosomePair_);
            (void) location;
            Gene::GetRandom(genePair).reset();
        }

        // gene loss
        if (Random::Percent() < 0.5) {
            chromosomePair_.erase(Random::Item(chromosomePair_).first);
        }

        // gene addition
        if (Random::Percent() < 0.5) {
            /*
             * FIXME perhaps the brain should be a fixed feature of the genome
             * and not a gene?
             *
             * Even if we parsed the genome and extracted the gene width, at
             * this point there'd be no way to tell if the brain width was going
             * to mutate later...
             *
             * Perhaps each gene needs a "BrainWidthMutated(newWidth)" function?
             */
            std::shared_ptr<Gene> randomGene = GeneFactory::RandomGene(NeuralNetwork::BRAIN_WIDTH);
            bool addToA = Random::Boolean();
            size_t randomLocation = Random::Number<size_t>(addToA ? aChromosomeRange_.Min() : bChromosomeRange_.Min(), addToA ? aChromosomeRange_.Max() : bChromosomeRange_.Max());
            addToA ? chromosomePair_[randomLocation].first = randomGene : chromosomePair_[randomLocation].second = randomGene;
        }

        // gene transposition
        if (Random::Percent() < 0.5) {
            auto& [ location, genePair ] = Random::Item(chromosomePair_);
            (void) location;
            std::shared_ptr<Gene>& randomGene = Gene::GetRandom(genePair);
            if (randomGene) {
                bool addToA = Random::Boolean();
                size_t randomLocation = Random::Number<size_t>(addToA ? aChromosomeRange_.Min() : bChromosomeRange_.Min(), addToA ? aChromosomeRange_.Max() : bChromosomeRange_.Max());
                addToA ? chromosomePair_[randomLocation].first = randomGene : chromosomePair_[randomLocation].second = randomGene;
                randomGene.reset();
            }
        }
    }

    // Remove any gene pairs that are no longer on either chromosome
    // Sadly std::remove_if doesn't work for std::map, perhaps future erase_if will...
    for (auto iter = chromosomePair_.begin(); iter != chromosomePair_.end();) {
        const unsigned& location = iter->first;
        if ((location < aChromosomeRange_.Min() && location < bChromosomeRange_.Min())
         || (location > aChromosomeRange_.Max() && location > bChromosomeRange_.Max())) {
            iter = chromosomePair_.erase(iter);
        } else {
            ++iter;
        }
    }

    for (auto& [location, genes ] : chromosomePair_) {
        auto& [ geneA, geneB ] = genes;
        if (geneA != nullptr && (location < aChromosomeRange_.Min() || location > aChromosomeRange_.Max())) {
            // trim genes outside of their chromosome range
            geneA = nullptr;
        } else if (geneA != nullptr && Random::Number(0.0, 1.0) < geneA->GetMutationProbability()) {
            geneA = geneA->Copy(MUTATIONS_PER_COPY);
        }
        if (geneB != nullptr && (location < bChromosomeRange_.Min() || location > bChromosomeRange_.Max())) {
            // trim genes outside of their chromosome range
            geneB = nullptr;
        } else if (geneB != nullptr && Random::Number(0.0, 1.0) < geneB->GetMutationProbability()) {
            geneB = geneB->Copy(MUTATIONS_PER_COPY);
        }
    }
}

ChromosomePair::ChromosomePair(const EoBE::Range<unsigned>& aRange, const EoBE::Range<unsigned>& bRange, std::map<unsigned, std::pair<std::shared_ptr<Gene>, std::shared_ptr<Gene> > >&& chromosomePair)
    : aChromosomeRange_(aRange)
    , bChromosomeRange_(bRange)
    , chromosomePair_(chromosomePair)
{
}
