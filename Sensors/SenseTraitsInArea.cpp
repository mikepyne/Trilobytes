#include "SenseTraitsInArea.h"

#include "Swimmer.h"

#include <QPainter>

SenseTraitsInArea::SenseTraitsInArea(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, double radius, const Transform& transform, std::vector<TraitNormaliser>&& toDetect)
    : SenseTraitsBase(network, outputConnections, owner, transform, std::move(toDetect))
    , senseRadius_(radius)
{
}

void SenseTraitsInArea::Draw(QPainter& paint) const
{
    Circle c = GetArea();
    paint.setPen(Qt::black);
    paint.setBrush(Qt::NoBrush);
    paint.drawEllipse(QPointF(c.x, c.y), senseRadius_, senseRadius_);
}

Circle SenseTraitsInArea::GetArea() const
{
    Point centre = ApplyOffset(owner_.GetLocation(), transform_.rotation + owner_.GetTransform().rotation, std::sqrt(GetDistanceSquare({ 0, 0 }, { transform_.x, transform_.y })));
    return { centre.x, centre.y, senseRadius_ };
}

void SenseTraitsInArea::FilterEntities(const EntityContainerInterface& entities, const std::function<void (const Entity&, const double&)>& forEachEntity) const
{
    const Circle senseArea = GetArea();
    const Point senseCentre = { senseArea.x, senseArea.y };
    const double senseRadiusSquare = std::pow(senseArea.radius, 2.0);
    entities.ForEachCollidingWith(senseArea, [&](const std::shared_ptr<Entity>& e)
    {
        // don't detect ourself
        if (e.get() != &owner_) {
            double distanceSquare = GetDistanceSquare(senseCentre, e->GetLocation());
            if (distanceSquare < senseRadiusSquare) {
                // Traits scaled by distance entity is from centre of sense
                forEachEntity(*e, 1.0 - (std::sqrt(distanceSquare) / senseArea.radius));
            }
        }
    });
}
