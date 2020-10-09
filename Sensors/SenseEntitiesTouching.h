#ifndef SENSEENTITIESTOUCHING_H
#define SENSEENTITIESTOUCHING_H

#include "Shape.h"
#include "SenseTraitsBase.h"

class QPainter;

class SenseEntitiesTouching final : public SenseTraitsBase {
public:
    SenseEntitiesTouching(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, Transform transform, std::vector<TraitNormaliser>&& toDetect);
    virtual ~SenseEntitiesTouching() override {}

    virtual std::string_view GetName() const override { return "SenseTraitsTouching"; }
    virtual void Draw(QPainter& paint) const override;

private:
    Point GetPoint() const;

    virtual void FilterEntities(const EntityContainerInterface& entities, const std::function<void (const Entity&, const double&)>& forEachEntity) const override;
};

#endif // SENSEENTITIESTOUCHING_H
