#ifndef ENERGYPOOL_H
#define ENERGYPOOL_H

#include <assert.h>
#include <algorithm>
#include <cstdint>

class EnergyPool {
public:
    EnergyPool(EnergyPool&& other);
    EnergyPool(const EnergyPool& other) = delete;
    EnergyPool& operator=(const EnergyPool& other) = delete;
    EnergyPool& operator=(EnergyPool&& other) = delete;
    ~EnergyPool();

    static EnergyPool& GetGlobalEnergyPool();

    void GiveTo(EnergyPool& toGiveTo, uint64_t quantityToGive);
    void Decay(uint64_t quantityToGiveToGlobalPool);

    uint64_t Quantity() const { return energy_; }
    EnergyPool CreateChild(uint64_t energyToTransfer);

private:
    uint64_t energy_;

    EnergyPool(uint64_t quantity);
    EnergyPool(EnergyPool& parent, uint64_t energyToTransfer);
};

#endif // ENERGYPOOL_H
