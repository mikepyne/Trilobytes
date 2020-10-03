#include "SenseEntitiesInArea.h"

#include "Swimmer.h"

#include <QPainter>

SenseEntitiesInArea::SenseEntitiesInArea(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, double maxDistance, const Transform& transform, double senseDistanceWeight, const std::vector<std::pair<double, Trait>>&& toDetect)
    : Sense(network, outputConnections, owner)
    , senseRadius_(maxDistance)
    , transform_(transform)
    , senseDistanceWeight_(senseDistanceWeight)
    , toDetect_(std::move(toDetect))
{
}

void SenseEntitiesInArea::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities) const
{
    for (auto& input : inputs) {
        input = 0.0;
    }

    Circle senseArea = GetArea();
    Point senseCentre = { senseArea.x, senseArea.y };
    double senseRadiusSquare = std::pow(senseArea.radius, 2.0);
    entities.ForEachCollidingWith(senseArea, [&](Entity& e)
    {
        // don't detect ourself
        if (&e != &owner_) {
            double distanceSquare = GetDistanceSquare(senseCentre, e.GetLocation());
            if (distanceSquare < senseRadiusSquare) {
                double distanceProportion = 1.0 - (distanceSquare / senseRadiusSquare);
                inputs.at(0) += distanceProportion * senseDistanceWeight_;
                size_t i = 0;
                for (auto& [traitWeight, trait ] : toDetect_) {
                    inputs.at(++i) += distanceProportion * traitWeight * e.GetTrait(trait);
                }
            }
        }
    });
}

void SenseEntitiesInArea::Draw(QPainter& paint) const
{
    Circle c = GetArea();
    paint.setPen(Qt::black);
    paint.setBrush(Qt::NoBrush);
    paint.drawEllipse(QPointF(c.x, c.y), senseRadius_, senseRadius_);
}

Circle SenseEntitiesInArea::GetArea() const
{
    Point centre = ApplyOffset(owner_.GetLocation(), transform_.rotation + owner_.GetTransform().rotation, std::sqrt(GetDistanceSquare({ 0, 0 }, { transform_.x, transform_.y })));
    return { centre.x, centre.y, senseRadius_ };
}
