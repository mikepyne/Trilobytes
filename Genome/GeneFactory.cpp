#include "GeneFactory.h"

#include "GenePigment.h"

std::shared_ptr<Gene> GeneFactory::Random()
{
    return std::make_shared<GenePigment>();
}
