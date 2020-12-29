#ifndef MEATCHUNK_H
#define MEATCHUNK_H

#include "Entity.h"

class MeatChunk : public Entity {
public:
    MeatChunk(const Energy& energy, const Transform& transform, const double& speed);
    ~MeatChunk() override;

    virtual std::string_view GetName() const override { return "MeatChunk"; }

protected:
    virtual void TickImpl(EntityContainerInterface& container, const UniverseParameters& universeParameters) override final;
    virtual void DrawImpl(QPainter& paint) override final;

private:
    static double GetRadius(const Energy& energy);
};

#endif // MEATCHUNK_H
