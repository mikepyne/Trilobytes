#include "EffectorMouth.h"

#include "Swimmer.h"
#include <QPainter>

EffectorMouth::EffectorMouth(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, Swimmer& owner, const Transform& transform)
    : Effector(network, inputConnections, owner)
    , radius_(3.5)
    , transform_(transform)
{
}

void EffectorMouth::Draw(QPainter& paint) const
{
    Circle mouth = GetGeometry();
    paint.setPen(Qt::white);
    paint.drawEllipse(mouth.x, mouth.y, mouth.radius, mouth.radius);
}

void EffectorMouth::PerformActions(const std::vector<double>& actionValues, const EntityContainerInterface& entities)
{
    entities.ForEachCollidingWith(GetGeometry(), [&](const std::shared_ptr<Entity>& other) -> void
    {
        if (actionValues.at(0) > 0.0) {
            if (other.get() != &owner_) {
                owner_.FeedOn(*other, other->GetEnergy() * actionValues.at(0));
            }
        }
    });
}

Circle EffectorMouth::GetGeometry() const
{
    Point centre = ApplyOffset(owner_.GetLocation(), transform_.rotation + owner_.GetTransform().rotation, GetDistance({ 0, 0 }, { transform_.x, transform_.y }));
    return { centre.x, centre.y, radius_ };



    // TODO seriously consider going back to the model where digestion is easy/100% and it is all about what the mouth is evolved to eat
    // It is too unfair that a mouth can still deall 100% damage to another swimmer even if none of the energy is even used


}
