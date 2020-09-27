#include "SenseEntitiesTouching.h"

#include "Swimmer.h"

#include <QPainter>

SenseEntitiesTouching::SenseEntitiesTouching(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, double offsetDistance, double offsetAngle, double genericDetectionWeight, const std::vector<std::pair<double, Trait> >&& toDetect)
    : Sense(network, outputConnections, owner)
    , offsetDistance_(offsetDistance)
    , offsetAngle_(offsetAngle)
    , genericDetectionWeight_(genericDetectionWeight)
{
}

void SenseEntitiesTouching::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const Sense::UniverseInfoStructRef& /*environment*/) const
{
    for (auto& input : inputs) {
        input = 0.0;
    }

    Point location = ApplyOffset(owner_.GetLocation(), offsetAngle_ + owner_.GetTransform().rotation, offsetDistance_);
    entities.ForEachCollidingWith(location, [&](Entity& e)
    {
        // don't detect ourself
        if (&e != &owner_) {
            inputs[0] += genericDetectionWeight_;
            size_t i = 0;
            for (auto& [traitWeight, trait ] : toDetect_) {
                inputs[++i] += traitWeight * e.GetTrait(trait);
            }
        }
    });
}

void SenseEntitiesTouching::Draw(QPainter& paint) const
{
    Point location = ApplyOffset(owner_.GetLocation(), offsetAngle_ + owner_.GetTransform().rotation, offsetDistance_);
    paint.setPen(QColor::fromRgb(255, 0, 0));
    paint.setBrush(QColor::fromRgb(0, 0, 0, 0));
    paint.drawEllipse(QPointF(location.x, location.y), 1.0, 1.0);
}
