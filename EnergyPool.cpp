#include "EnergyPool.h"

EnergyPool::EnergyPool(EnergyPool&& other)
    : energy_(0.0)
{
    other.GiveTo(*this, other.energy_);
}

EnergyPool::~EnergyPool()
{
    GiveTo(GetGlobalEnergyPool(), energy_);
}

EnergyPool& EnergyPool::GetGlobalEnergyPool()
{
    static EnergyPool globalPool(1000.0);
    return globalPool;
}

void EnergyPool::GiveTo(EnergyPool& other, double quantityToGive)
{
    quantityToGive = std::clamp(quantityToGive, 0.0, energy_);
    energy_ -= quantityToGive;
    other.energy_ += quantityToGive;
}

void EnergyPool::Decay(double quantityToGiveToGlobalPool)
{
    GiveTo(GetGlobalEnergyPool(), quantityToGiveToGlobalPool);
}

EnergyPool EnergyPool::CreateChild(double energyToTransfer)
{
    return EnergyPool(*this, energyToTransfer);
}

EnergyPool::EnergyPool(double quantity)
    : energy_(quantity)
{
    static bool createdGlobalPool = false;
    if (createdGlobalPool) {
        assert(false && "There should only be 1 global energy pool");
    }
    createdGlobalPool = true;
}

EnergyPool::EnergyPool(EnergyPool& parent, double energyToTransfer)
    : energy_(0.0)
{
    parent.GiveTo(*this, energyToTransfer);
}
