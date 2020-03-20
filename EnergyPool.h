#ifndef ENERGYPOOL_H
#define ENERGYPOOL_H

#include <assert.h>
#include <algorithm>

class EnergyPool {
public:
    EnergyPool(EnergyPool&& other);
    EnergyPool(const EnergyPool& other) = delete;
    EnergyPool& operator=(const EnergyPool& other) = delete;
    EnergyPool& operator=(EnergyPool&& other) = delete;
    ~EnergyPool();

    static EnergyPool& GetGlobalEnergyPool();

    void GiveTo(EnergyPool& toGiveTo, double quantityToGive);
    void Decay(double quantityToGiveToGlobalPool);

    double Quantity() const { return energy_; }
    EnergyPool CreateChild(double energyToTransfer);

private:
    double energy_;

    EnergyPool(double quantity);
    EnergyPool(EnergyPool& parent, double energyToTransfer);
};

#endif // ENERGYPOOL_H
