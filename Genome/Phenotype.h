#ifndef PHENOTYPE_H
#define PHENOTYPE_H

#include "Sensors/Sense.h"
#include "NeuralNetwork.h"

#include <QColor>

#include <memory>
#include <vector>

struct Phenotype {
public:
    QColor colour;
    std::vector<std::shared_ptr<Sense>> senses;
    std::shared_ptr<NeuralNetwork> brain;
};

#endif // PHENOTYPE_H
