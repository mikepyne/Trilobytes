#include "GeneFactory.h"

#include "GenePigment.h"

std::shared_ptr<Gene> GeneFactory::Random()
{
    // TODO convert other traits to genes

    return std::make_shared<GenePigment>();
}
