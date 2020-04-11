#include "ChromosomePair.h"

#include "Random.h"

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
    // TODO more closely consider chromosome sizes and how crossover affects that
    // TODO make sure genes don't have locations outside of the chromosome range
    // TODO crossover more likely at tips of chromosomes?
    size_t crossoverCount = Random::Poisson(newPair.size() / 3); // TODO crossover count not related to gene count
    std::vector<unsigned> crossoverLocations =  Random::Numbers(crossoverCount, std::min(newAChromosomeRange_.Min(), newBChromosomeRange_.Min()), std::min(newAChromosomeRange_.Max(), newBChromosomeRange_.Max()));

    bool swap = Random::Boolean();
    auto swapLocation = crossoverLocations.cbegin();
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

    return { ChromosomePair(newAChromosomeRange_, newBChromosomeRange_, std::move(newPair)) };
}

void ChromosomePair::ForEach(const std::function<void (const Gene&)>& action) const
{
    for (auto& [ location, pair ] : chromosomePair_) {
        (void) location;
        action(*Gene::GetMostDominant(pair.first, pair.second));
    }
}

ChromosomePair::ChromosomePair(const EoBE::Range<unsigned>& aRange, const EoBE::Range<unsigned>& bRange, std::map<unsigned, std::pair<std::shared_ptr<Gene>, std::shared_ptr<Gene> > >&& chromosomePair)
    : aChromosomeRange_(aRange)
    , bChromosomeRange_(bRange)
    , chromosomePair_(chromosomePair)
{
}
