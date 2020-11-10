#include "Utils.h"

namespace EoBE {

void RollingStatistics::AddValue(const double& value)
{
    count_++;
    sumOfValues_ += value;
    sumOfValuesSquared_ += std::pow(value, 2);
    min_ = std::min(min_, value);
    max_ = std::max(max_, value);
}

double RollingStatistics::StandardDeviation() const
{
    if (count_ > 1) {
        return std::sqrt((sumOfValuesSquared_ - (std::pow(sumOfValues_, 2) / count_)) / count_);
    } else {
        return 0;
    }
}

RangeConverter::RangeConverter(Range<double> from, Range<double> to)
    : from_(from)
    , to_(to)
{
}

double RangeConverter::Convert(const double& value) const
{
    double proportion = (value - from_.First()) / from_.ValueDifference();
    return to_.First() + (proportion * to_.ValueDifference());
}

double RangeConverter::ConvertAndClamp(const double& value) const
{
    double proportion = (value - from_.First()) / from_.ValueDifference();
    return std::clamp(to_.First() + (proportion * to_.ValueDifference()), to_.Min(), to_.Max());
}

} // namespace EoBE
