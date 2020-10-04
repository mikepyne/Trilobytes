#include "SenseEntityRaycast.h"

#include "Swimmer.h"

#include <QPainter>

SenseEntityRaycast::SenseEntityRaycast(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, double maxDistance, double angle, double genericWeight, const std::vector<std::pair<double, Trait> >&& traits)
    : Sense(network, outputConnections, owner)
    , rayCastDistance_(maxDistance)
    , rayCastAngle_(angle)
    , genericWeight_(genericWeight)
    , traits_(traits)
{
}

void SenseEntityRaycast::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities) const
{
    Line rayCastLine = GetLine();
    std::shared_ptr<Entity> nearestEntity;
    double distanceToNearest = 0.0;
    entities.ForEachCollidingWith(rayCastLine, [&](const std::shared_ptr<Entity>& e)
    {
        // don't detect ourself
        if (e.get() != &owner_) {
            double distance = std::sqrt(GetDistanceSquare(owner_.GetLocation(), e->GetLocation())) - e->GetRadius();
            if (nearestEntity == nullptr || distance < distanceToNearest) {
                nearestEntity = e;
                distanceToNearest = distance;
            }
        }
    });

    if (nearestEntity != nullptr) {
        inputs.at(0) = (distanceToNearest / rayCastDistance_) * genericWeight_;
        size_t i = 0;
        for (auto& [ traitWeight, trait] : traits_) {
            inputs.at(++i) = traitWeight * nearestEntity->GetTrait(trait);
        }
    }
}

void SenseEntityRaycast::Draw(QPainter& paint) const
{
    auto l = GetLine();
    paint.setPen(QColor::fromRgb(0, 0, 0));
    paint.drawLine(QLineF(l.a.x, l.a.y, l.b.x, l.b.y));
}

Line SenseEntityRaycast::GetLine() const
{
    Point begin = ApplyOffset(owner_.GetLocation(), rayCastAngle_ + owner_.GetTransform().rotation, owner_.GetRadius());
    Point end = ApplyOffset(begin, rayCastAngle_ + owner_.GetTransform().rotation, rayCastDistance_);
    return { begin, end };
}
