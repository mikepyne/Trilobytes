#ifndef SENSEENTITYDISTANCE_H
#define SENSEENTITYDISTANCE_H

#include "Sense.h"
#include "Shape.h"
#include "Utils.h"

#include <optional>

#include "Entity.h"
#include "EntityContainerInterface.h"

/**
 * Used to check if one or a number of entities of any or specific types are
 * within a given area, weighted based on their proximity to the center of the
 * sense.
 */
class SenseEntityDistance : public Sense {
private:
    using FilterFunction = std::function<std::optional<std::pair<size_t, double>>(Entity&)>;

    /*
     * Recursive variadic template function
     * Index will match the number of times we had to recurse to find the match,
     * or the number of items in Rest
     */
    template<typename First, typename... Rest>
    static bool MatchesAnyOf(const Entity& e, unsigned& index)
    {
        static_assert(!(std::is_same<Entity, First>() && sizeof...(Rest) != 0), "All types following Entity have no chance of being detected! If generically detecting all entities, make sure that is the final type category.");

        // Check if it matches the First type, if not, recurse (if more Types available) and count times recursed
        if constexpr (std::is_same<Entity, First>()) {
            return true;
        } else if (dynamic_cast<const First*>(&e)) {
            return true;
        } else if constexpr(sizeof...(Rest) > 0) {
            ++index;
            return MatchesAnyOf<Rest...>(e, index);
        }
        return false;
    }

    /*
     * Don't trust a user to construct one of these properly, keep it private!
     * (FilterFunc relies on inputCount being correct)
     */
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
    SenseEntityDistance(Entity& owner, double xOffset, double yOffset, double range, CustomFilter&& detectionParameters = MakeDefaultFilter(1.0))
        : Sense(owner, detectionParameters.inputCount_, detectionParameters.hiddenLayers_)
        , entityNames_(detectionParameters.typeNames_ + "Detector")
        , xOffset_(xOffset)
        , yOffset_(yOffset)
        , range_(range)
        , entityFilter_(std::move(detectionParameters.filter_))
    {
    }

    /*
     * Detects any entities of any type.
     */
    static CustomFilter MakeDefaultFilter(double detectionQuantity = 1.0);

    /*
     * Yike... took a day to write, not proud of how easy this code is to
     * understand, but it certainly makes using it hard to use incorrectly, and
     * easy to read the code using it.
     *
     * Basically our filer needs to tell us whether it found a type we care
     * about AND which input neuron to prime AND the value to apply to the input
     * neuron. It does this by returning a std::optional (i.e. only populated if
     * we found a type we were looking for) and the optional containing the pair
     * of values, networkInputIndex and the valueToAddToInput.
     */
    template <typename... Es>
    static CustomFilter MakeCustomFilter(unsigned hiddenLayers, std::array<double, sizeof...(Es)> detectionValues)
    {
        return {
            sizeof...(Es),
            hiddenLayers,
            EoBE::TypeNames<Es...>("&"),
            [=](Entity& e) -> std::optional<std::pair<size_t, double>>
            {
                unsigned index = 0;
                if (MatchesAnyOf<Es...>(e, index)) {
                    return std::make_pair(index, detectionValues[index]);
                }
                return { };
            }
        };
    }

    virtual std::string_view GetName() const override { return entityNames_; }
    virtual void Draw(QPainter& paint) const override;

private:
    std::string entityNames_;
    double xOffset_;
    double yOffset_;
    double range_;
    FilterFunction entityFilter_;

    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseInfoStructRef& environment) override final;
};

#endif // SENSEENTITYDISTANCE_H
