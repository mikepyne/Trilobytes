#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <functional>
#include <cmath>
#include <iostream>
#include <assert.h>

namespace EoBE {

///
/// Constants
///
constexpr double Pi = 3.141592653589793;
constexpr double Tau = Pi * 2;

///
/// Types
///
template <typename T>
class Range {
public:
    Range(const T& first, const T& last)
        : first_(first)
        , last_(last)
    {}

    const T& Min() const { return std::min(first_, last_); }
    const T& Max() const { return std::max(first_, last_); }
    T ValueRange() const { return Max() - Min(); }
    const T& First() const { return first_; }
    const T& Last() const { return last_; }

    bool Contains(const T& value) const { return value >= Min() && value <= Max(); }

    double Similarity(const Range<T>& other) const
    {
        double overlap = static_cast<double>(std::min(Max(), other.Max()) - std::max(Min(), other.Min()));
        if (overlap > 0.0) {
            double combinedRange = static_cast<double>(std::max(Max(), other.Max()) - std::min(Min(), other.Min()));
            return overlap / combinedRange;
        } else {
            return 1.0;
        }
    }

    void ExpandToContain(const T& newValue)
    {
        if (first_ > last_) {
            if (newValue > first_) {
                first_ = newValue;
            }
            if (newValue < last_) {
                last_ = newValue;
            }
        } else {
            if (newValue > last_) {
                last_ = newValue;
            }
            if (newValue < first_) {
                first_ = newValue;
            }
        }
    }
    void SetFirst(const T& newFirst) { first_ = newFirst; }
    void SetLast(const T& newLast) { last_ = newLast; }
    void SetRange(const T& first, const T& last)
    {
        first_ = first;
        last_ = last;
    }

    bool operator>(const Range<T>& other) const { return Min() > other.Min() || (Min() == other.Min() && Max() > other.Max()); }
    bool operator<(const Range<T>& other) const { return Min() < other.Min() || (Min() == other.Min() && Max() < other.Max()); }
    bool operator==(const Range<T>& other) const { return first_ == other.first_ && last_ == other.last_; }
    bool operator>=(const Range<T>& other) const { return *this > other || *this == other; }
    bool operator<=(const Range<T>& other) const { return *this < other || *this == other; }

private:
    T first_;
    T last_;
};

///
/// Helpers
///

template <typename T1, typename T2>
void IterateBoth(std::vector<T1>& a, std::vector<T2>& b, std::function<void(T1& a, T2& b)>&& action)
{
    auto aIter = a.begin();
    auto bIter = b.begin();
    for (; aIter != a.end() && bIter != b.end() ; ++aIter, ++bIter) {
        action(*aIter, *bIter);
    }
}

template <typename T1, typename T2>
void IterateBoth(const std::vector<T1>& a, const std::vector<T2>& b, std::function<void(const T1& a, const T2& b)>&& action)
{
    auto aIter = a.begin();
    auto bIter = b.begin();
    for (; aIter != a.end() && bIter != b.end() ; ++aIter, ++bIter) {
        action(*aIter, *bIter);
    }
}

template <typename T1, typename T2>
void IterateBoth(const std::vector<T1>& a, std::vector<T2>& b, std::function<void(const T1& a, T2& b)>&& action)
{
    auto aIter = a.begin();
    auto bIter = b.begin();
    for (; aIter != a.end() && bIter != b.end() ; ++aIter, ++bIter) {
        action(*aIter, *bIter);
    }
}

template <typename T1, typename T2>
void IterateBoth(std::vector<T1>& a, const std::vector<T2>& b, std::function<void(T1& a, const T2& b)>&& action)
{
    auto aIter = a.begin();
    auto bIter = b.begin();
    for (; aIter != a.end() && bIter != b.end() ; ++aIter, ++bIter) {
        action(*aIter, *bIter);
    }
}

template <typename T>
std::vector<T> CreateSeries(T firstValue, uint64_t count, std::function<T(const T& previous)>&& nextValue = [](const T& previous){ return previous + 1; })
{
    std::vector<T> series;
    series.resize(count);
    std::generate_n(std::begin(series), count, [&]() -> T
    {
        T temp = firstValue;
        firstValue = nextValue(temp);
        return temp;
    });
    return series;
}

template <typename T>
std::vector<T> CreateSeries(T firstValue, T lastValue, uint64_t count)
{
    assert(count >= 2);
    long double soFar = firstValue;
    long double increment = (lastValue - firstValue) / (count - 1);
    return CreateSeries<T>(soFar, count, [&](const T& /*prev*/) -> T
    {
        soFar += increment;
        if constexpr (std::is_integral<T>::value) {
            return static_cast<T>(std::round(soFar));
        } else {
            return static_cast<T>(soFar);
        }
    });
}

template <typename T>
void Print(const std::vector<T>& v, std::ostream& out)
{
    out << "{ ";
    for (const auto& i : v) {
        out << i << " ";
    }
    out << "}" << std::endl;
}

// credit https://stackoverflow.com/users/3624760/lyberta
template <typename T>
constexpr std::string_view TypeName()
{
    std::string_view name, prefix, suffix;
#ifdef __clang__
    name = __PRETTY_FUNCTION__;
    prefix = "std::string_view EoBE::TypeName() [T = ";
    suffix = "]";
#elif defined(__GNUC__)
    name = __PRETTY_FUNCTION__;
    prefix = "constexpr std::string_view EoBE::TypeName() [with T = ";
    suffix = "; std::string_view = std::basic_string_view<char>]";
#elif defined(_MSC_VER)
    name = __FUNCSIG__;
    prefix = "class std::basic_string_view<char,struct std::char_traits<char> > __cdecl EoBE::TypeName<";
    suffix = ">(void)";
#endif
    name.remove_prefix(prefix.size());
    name.remove_suffix(suffix.size());
    return name;
}

template<typename Arg, typename... Args>
std::string TypeNames(const std::string& seperator)
{
    std::stringstream result;
    result << TypeName<Arg>();
    if constexpr (sizeof...(Args) > 0) {
        result << seperator << TypeNames<Args...>(seperator);
    }
    return result.str();
}

} // namespace EoBE

#endif // UTILS_H
