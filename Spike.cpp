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
            swimmer->ApplyDamage(5 * std::pow(swimmer->GetVelocity(), 2.0));
        }
    });
}

void Spike::DrawImpl(QPainter& paint)
{
    for (double rotation = 0.0; rotation < EoBE::Tau; rotation += EoBE::Tau / 17) {
        Point tip = ApplyOffset({ GetLocation().x, GetLocation().y }, rotation, GetRadius());
        paint.drawLine(QLineF(GetLocation().x, GetLocation().y, tip.x, tip.y));
    }
    paint.drawEllipse(QPointF(GetLocation().x, GetLocation().y), GetRadius() / 2, GetRadius() / 2);
}
