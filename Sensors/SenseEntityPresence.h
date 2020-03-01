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
private:
    using FilterFunction = std::function<std::optional<std::pair<size_t, double>>(Entity&)>;

    struct CustomFilter {
        const unsigned inputCount_;
        const unsigned hiddenLayers_;
        std::string typeNames_;
        const FilterFunction filter_;
    };

public:
    /**
     * Constructs a simple detection sensor for any Entity in range, regardless
     * of type.
     *
     * @param owner - The Entity this sensor belongs to
     * @param xOffset - The position of this sensor relative to the owner
     * @param yOffset - The position of this sensor relative to the owner
     * @param range - The size of the circular area of this sensor
     * @param detectionParameters
     *        - This defines which Entity types trigger this sense, and the
     *          impact each entity of each type on the sensor value. By default
     *          any entity of any type is detected as 1.0 (max value) so the
     *          sense is binary anything or nothing detected.
     */
    SenseEntityPresence(Entity& owner, double xOffset, double yOffset, double range, CustomFilter&& detectionParameters = MakeDefaultFilter(1.0))
        : Sense(owner, detectionParameters.inputCount_, detectionParameters.hiddenLayers_)
        , name_(detectionParameters.typeNames_ + "Detector")
        , xOffset_(xOffset)
        , yOffset_(yOffset)
        , range_(range)
        , filter_(std::move(detectionParameters.filter_))
    {
    }

    static CustomFilter MakeDefaultFilter(double detectionQuantity = 1.0);

    template <typename E1>
    static CustomFilter MakeCustomFilter(double detectionQuantity)
    {
        return {
            1,
            0,
            std::string(EoBE::TypeName<E1>()),
            [=](Entity& e) -> std::optional<std::pair<size_t, double>>
            {
                if (dynamic_cast<E1*>(&e)) {
                    return std::make_pair(0u, detectionQuantity);
                }
                return { };
            }
        };
    }

    template <typename E1, typename E2>
    static CustomFilter MakeCustomFilter(double type1DetectionQuantity, double type2DetectionQuantity)
    {
        return {
            2,
            1,
            std::string(EoBE::TypeName<E1>()) + "&" + std::string(EoBE::TypeName<E2>()),
            [=](Entity& e) -> std::optional<std::pair<size_t, double>>
            {
                if (dynamic_cast<E1*>(&e)) {
                    return std::make_pair(0u, type1DetectionQuantity);
                } else if (dynamic_cast<E2*>(&e)) {
                    return std::make_pair(1u, type2DetectionQuantity);
                }
                return { };
            }
        };
    }

    virtual std::string GetName() const override { return name_; }
    virtual void Draw(QPainter& paint) const override {}

private:
    std::string name_;
    double xOffset_;
    double yOffset_;
    double range_;
    FilterFunction filter_;

    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseInfoStructRef& environment) override final;
};

#endif // SENSOR_H
