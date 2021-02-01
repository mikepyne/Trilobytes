#include "Spike.h"
#include "Swimmer.h"

#include <QPainter>

Spike::Spike(const Transform& transform) :
    Entity(1_uj, transform, 12, QColor::fromRgb(225, 225, 225))
{
}

Spike::~Spike()
{
}

void Spike::TickImpl(EntityContainerInterface& container, const UniverseParameters& /*universeParameters*/)
{
    container.ForEachCollidingWith(Circle{ GetLocation().x, GetLocation().y, GetRadius() }, [](const std::shared_ptr<Entity>& entity)
    {
        if (Swimmer* swimmer = dynamic_cast<Swimmer*>(entity.get())) {
            // FIXME entity rotation and direction of movement may not actually be the same! (they were when writing, but perhaps that should change!)
            Vec2 victimVec = GetMovementVector(entity->GetTransform().rotation, entity->GetVelocity());

            auto [ contactBearing, contactVelocity ] = DeconstructMovementVector({ -victimVec.x, -victimVec.y });

            double collisionBearing = std::fmod(std::abs(contactBearing), Tril::Tau);
            double directHitProportion = std::max(0.0, ((std::abs(collisionBearing - Tril::Pi) / Tril::Pi) - 0.5) * 2);
            swimmer->ApplyDamage(directHitProportion * (5 * std::pow(contactVelocity, 2.0)));
        }
    });
}

void Spike::DrawImpl(QPainter& paint)
{
    for (double rotation = 0.0; rotation < Tril::Tau; rotation += Tril::Tau / 17) {
        Point tip = ApplyOffset({ GetLocation().x, GetLocation().y }, rotation, GetRadius());
        paint.drawLine(QLineF(GetLocation().x, GetLocation().y, tip.x, tip.y));
    }
    paint.drawEllipse(QPointF(GetLocation().x, GetLocation().y), GetRadius() / 2, GetRadius() / 2);
}
