#ifndef SENSOR_H
#define SENSOR_H

#include "Sense.h"
#include "Shape.h"
#include "Utils.h"

#include <optional>

#include "Entity.h"
#include "EntityContainerInterface.h"

/**
 * Used to check if one or a number of entities of any or specific types are
 * within a given
 */
class SenseEntityPresence : public Sense {
public:
    /**
     * Constructs a simple binary, detection on or off, sensor for a particular
     * entity type, e.g. food.
     */
    SenseEntityPresence(Entity& owner, double xOffset, double yOffset, double range);

    template <typename EntityType>
    static SenseEntityPresence MakeSingleEntityTypeSensor(Entity& owner, double xOffset, double yOffset, double range)
    {
        return SenseEntityPresence(owner, xOffset, yOffset, range, 1, 0, std::string(EoBE::TypeName<EntityType>()) + "PresenceSensor", [](Entity& e) -> std::optional<size_t>
        {
            if (dynamic_cast<EntityType*>(&e)) {
                return { 0 };
            }
            return { };
        });
    }

    virtual std::string GetName() const override { return name_; }
    virtual void Draw(QPainter& paint) const override {}

private:
    std::string name_;
    double xOffset_;
    double yOffset_;
    double range_;
    double detectionQuantity_;
    std::function<std::optional<size_t>(Entity&)> filter_;

    SenseEntityPresence(Entity& owner, double xOffset, double yOffset, double range, unsigned inputs, unsigned hiddenLayers, const std::string& name, std::function<std::optional<size_t>(Entity&)>&& filter);

    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseInfoStructRef& environment) override final;
};

#endif // SENSOR_H
