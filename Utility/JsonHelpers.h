#ifndef JSONHELPERS_H
#define JSONHELPERS_H

#include "Libs/nlohmann/json.hpp"

class JsonHelpers {
public:

/**
 * Can return true even if "target != toMatch" when target is a more inclusive
 * arithmetic type, i.e. when target is float, toMatch can be an int or unsigned
 */
static bool MatchType(nlohmann::json::value_t target, nlohmann::json::value_t toMatch);

/**
 * Ensures the supplied json is an object that contains exactly the specified
 * keys, of the correct types
 */
static bool ValidateJsonObject(const nlohmann::json& object, std::initializer_list<std::pair<std::string, nlohmann::json::value_t>>&& expectedEntries);

/**
 * Ensures the supplied array contains only the specified value type. For multi-
 * dimensional arrays, the dimensions are checked to ensure they are all the
 * same size.
 */
static bool ValidateJsonArray(const nlohmann::json& array, nlohmann::json::value_t valueType, unsigned dimensions = 1);

/**
 * Does ABSOLUTELY no checking, use ValidateJsonArray first please...
 * Returns an N dimensional array populated with the json data
 */
template <typename ValueType, size_t dimensions = 1>
static auto ParseJsonArray(const nlohmann::json& array)
{
    static_assert(dimensions > 0);

    // Utilise constexpr if and auto return type to return different types
    if constexpr (dimensions == 1) {
        /// Return a vector of ValueType

        std::vector<ValueType> items;
        for (const auto& item : array) {
            items.push_back(item.get<ValueType>());
        }
        return items;
    } else {
        /// Return a vector of a vector of... recursively until dimensions == 1

        // Increase the number of dimensions in our returned vector
        std::vector<decltype(ParseJsonArray<ValueType, dimensions - 1>({}))> container;

        for (const auto& subArray : array) {
            container.push_back(ParseJsonArray<ValueType, dimensions - 1>(subArray));
        }

        return container;
    }
}

};

#endif // JSONHELPERS_H
