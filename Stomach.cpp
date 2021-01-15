#include "Stomach.h"


Stomach::Stomach()
    : enzymeCount_(0)
    , total_(1.0)
    , digestionProportion_({ {DietType::Herbivore, 0.9}, {DietType::Carnivore, 0.1}, {DietType::Eggivore, 0.0} })
{
}

void Stomach::AddEnzyme(const DietType& target)
{
    ++enzymeCount_;
    total_ += 2.0;
    digestionProportion_[target] += 2.0;
}

double Stomach::GetDigestionEfficiency(const DietType& target) const
{
    return digestionProportion_.count(target) == 0 ? 0.0 : std::abs(digestionProportion_.at(target)) / total_;
}

Energy Stomach::GetMetabolicCost() const
{
    return std::pow(enzymeCount_, 1.2) * 5_uj;
}
