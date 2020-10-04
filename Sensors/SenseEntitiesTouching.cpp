#include "SenseEntitiesTouching.h"

#include "Swimmer.h"

#include <QPainter>

SenseEntitiesTouching::SenseEntitiesTouching(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, Transform transform, double genericDetectionWeight, const std::vector<std::pair<double, Trait> >&& toDetect)
    : Sense(network, outputConnections, owner)
    , transform_(transform)
    , genericDetectionWeight_(genericDetectionWeight)
    , toDetect_(std::move(toDetect))
{
}

void SenseEntitiesTouching::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities) const
{
    Point location = ApplyOffset(owner_.GetLocation(), transform_.rotation + owner_.GetTransform().rotation, std::sqrt(GetDistanceSquare(owner_.GetLocation(), { owner_.GetLocation().x + transform_.x, owner_.GetLocation().y + transform_.y })));
    entities.ForEachCollidingWith(location, [&](const std::shared_ptr<Entity>& e)
    {
        // don't detect ourself
        if (e.get() != &owner_) {
            inputs.at(0) += genericDetectionWeight_;
            size_t i = 0;
            for (auto& [traitWeight, trait ] : toDetect_) {
                inputs.at(++i) += traitWeight * e->GetTrait(trait);
            }
        }
    });
}

void SenseEntitiesTouching::Draw(QPainter& paint) const
{
    Point location = ApplyOffset(owner_.GetLocation(), transform_.rotation + owner_.GetTransform().rotation, std::sqrt(GetDistanceSquare(owner_.GetLocation(), { owner_.GetLocation().x + transform_.x, owner_.GetLocation().y + transform_.y })));
    paint.setPen(QColor::fromRgb(255, 0, 0));
    paint.setBrush(QColor::fromRgb(0, 0, 0, 0));
    paint.drawEllipse(QPointF(location.x, location.y), 1.0, 1.0);
}
