#include "EnergyPool.h"

EnergyPool::EnergyPool(EnergyPool&& other)
    : energy_(0)
{
    other.GiveTo(*this, other.energy_);
}

EnergyPool::~EnergyPool()
{
    GiveTo(GetGlobalEnergyPool(), energy_);
}

EnergyPool& EnergyPool::GetGlobalEnergyPool()
{
    static EnergyPool globalPool(100000000);
    return globalPool;
}

void EnergyPool::GiveTo(EnergyPool& other, uint64_t quantityToGive)
{
    quantityToGive = std::clamp(quantityToGive, static_cast<uint64_t>(0), energy_);
    energy_ -= quantityToGive;
    other.energy_ += quantityToGive;
}

void EnergyPool::Decay(uint64_t quantityToGiveToGlobalPool)
{
    GiveTo(GetGlobalEnergyPool(), quantityToGiveToGlobalPool);
}

EnergyPool EnergyPool::CreateChild(uint64_t energyToTransfer)
{
    return EnergyPool(*this, energyToTransfer);
}

EnergyPool::EnergyPool(uint64_t quantity)
    : energy_(quantity)
{
    static bool createdGlobalPool = false;
    if (createdGlobalPool) {
        assert(false && "There should only be 1 global energy pool");
    }
    createdGlobalPool = true;
}

EnergyPool::EnergyPool(EnergyPool& parent, uint64_t energyToTransfer)
    : energy_(0)
{
    parent.GiveTo(*this, energyToTransfer);
}
