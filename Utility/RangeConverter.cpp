#include "RangeConverter.h"

#include "Utility/JsonHelpers.h"

using namespace nlohmann;

namespace Tril {

RangeConverter::RangeConverter(Range<double> from, Range<double> to)
    : from_(from)
    , to_(to)
{
}

json RangeConverter::Serialise(const RangeConverter& rangeConverter)
{
    return {
        {KEY_FROM, Range<double>::Serialise(rangeConverter.from_)},
        {KEY_TO, Range<double>::Serialise(rangeConverter.to_)}
    };
}

std::optional<RangeConverter> RangeConverter::Deserialise(const json& serialised)
{
    if (JsonHelpers::ValidateJsonObject(serialised, { {KEY_FROM, json::value_t::object}, {KEY_TO, json::value_t::object}})) {
        std::optional<Range<double>> from = Range<double>::Deserialise(serialised.at(KEY_FROM));
        std::optional<Range<double>> to = Range<double>::Deserialise(serialised.at(KEY_TO));
        if (from.has_value() && to.has_value()) {
            return { RangeConverter(from.value(), to.value()) };
        }
    }
    return {};
}

double RangeConverter::Convert(const double& value) const
{
    double proportion = (value - from_.First()) / from_.ValueDifference();
    return to_.First() + (proportion * to_.ValueDifference());
}

double RangeConverter::ConvertAndClamp(const double& value) const
{
    return std::clamp(Convert(value), to_.Min(), to_.Max());
}





} // end namespace Tril
