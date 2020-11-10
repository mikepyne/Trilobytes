#ifndef GENEFACTORY_H
#define GENEFACTORY_H

#include "Genome.h"
#include "Gene.h"

#include <memory>

class GeneFactory {
public:
    static std::shared_ptr<Genome> DefaultGenome();
    static std::shared_ptr<Genome> RandomGenome();
    static std::shared_ptr<Gene> RandomGene(unsigned brainWidth);
};

#endif // GENEFACTORY_H
