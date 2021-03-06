#ifndef RANGECONVERTER_H
#define RANGECONVERTER_H

#include "Utility/Range.h"
#include "Libs/nlohmann/json.hpp"

#include <optional>

namespace Tril {

class RangeConverter {
public:
    RangeConverter(Range<double> from, Range<double> to);

    static nlohmann::json Serialise(const RangeConverter& rangeConverter);
    static std::optional<RangeConverter> Deserialise(const nlohmann::json& serialised);

    double Convert(const double& value) const;
    double ConvertAndClamp(const double& value) const;
    const Range<double>& GetFrom() const { return from_; }
    const Range<double>& GetTo() const { return to_; }

private:
    static const inline std::string KEY_FROM = "From";
    static const inline std::string KEY_TO = "To";

    Range<double> from_;
    Range<double> to_;
};

} // end namespace Tril

#endif // RANGECONVERTER_H
