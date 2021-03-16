#include "Transform.h"

using namespace nlohmann;

json Transform::Serialise(const Transform& transform)
{
    return {
        {KEY_X, transform.x},
        {KEY_Y, transform.y},
        {KEY_Rotation, transform.rotation},
    };
}

std::optional<Transform> Transform::Deserialise(const json& transform)
{
    if (JsonHelpers::ValidateJsonObject(transform, { {KEY_X, json::value_t::number_float}, {KEY_Y, json::value_t::number_float}, {KEY_Rotation, json::value_t::number_float}})) {
        return { {transform.at(KEY_X).get<double>(), transform.at(KEY_Y).get<double>(), transform.at(KEY_Rotation).get<double>()} };
    }
    return std::nullopt;
}
