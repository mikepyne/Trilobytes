#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Shape.h"
#include "Utility/JsonHelpers.h"

#include <nlohmann/json.hpp>

/**
 * @brief Placeholder for a proper matrix based coordinate system
 */
struct Transform {
public:
    double x;
    double y;
    double rotation;

    Transform operator+(const Transform& other) const
    {
        return { x + other.x, y + other.y, rotation + other.rotation };
    }

    static nlohmann::json Serialise(const Transform& transform);
    static std::optional<Transform> Deserialise(const nlohmann::json& transform);

private:
    static const inline std::string KEY_X = "x";
    static const inline std::string KEY_Y = "y";
    static const inline std::string KEY_Rotation = "Rotation";
};

#endif // TRANSFORM_H
