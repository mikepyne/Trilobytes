#ifndef GENEFACTORY_H
#define GENEFACTORY_H

#include "Gene.h"

#include <memory>

class GeneFactory {
public:
    static std::shared_ptr<Gene> Random();
};

#endif // GENEFACTORY_H
