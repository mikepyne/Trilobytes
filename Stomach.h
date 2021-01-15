#ifndef STOMACH_H
#define STOMACH_H

#include "Energy.h"

#include <map>
#include <cmath>

enum class DietType {
    Herbivore,
    Carnivore,
    Eggivore,
};

/**
 * The aim here is to encourage specialisation in a particular food type, by
 * recucing energy obtained from non-target entities.
 *
 * Initially a plain ratio seems reasonable, however it may be nice to allow
 * swimmers to evolve the ability to efficiently digest more than one type of
 * Entity at the cost of an increased metabolic rate.
 */
class Stomach {
public:
    /**
     * By default prioritises herbivorous diet, with a small ammount of omnivory
     */
    Stomach();

    void AddEnzyme(const DietType& target);

    double GetDigestionEfficiency(const DietType& target) const;
    Energy GetMetabolicCost() const;

private:
    unsigned enzymeCount_ = 0;
    double total_ = 0.0;
    std::map<DietType, double> digestionProportion_;
};

#endif // STOMACH_H
