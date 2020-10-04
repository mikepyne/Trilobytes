#include "SenseEntityRaycast.h"

#include "Swimmer.h"

#include <QPainter>

SenseEntityRaycast::SenseEntityRaycast(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, double maxDistance, double angle, const std::vector<Trait>&& toDetect)
    : Sense(network, outputConnections, owner)
    , rayCastDistance_(maxDistance)
    , rayCastAngle_(angle)
    , toDetect_(toDetect)
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
        inputs.at(0) = distanceToNearest / rayCastDistance_;
        for (unsigned i = 0; i < toDetect_.size(); i++) {
            inputs.at(i + 1) = nearestEntity->GetTrait(toDetect_[i]);
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
