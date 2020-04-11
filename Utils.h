#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <string_view>

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

    bool operator>(const Range<T>& other) const { return first_ > other.first_ || (first_ == other.first_ && last_ > other.last_); }
    bool operator<(const Range<T>& other) const { return first_ < other.first_ || (first_ == other.first_ && last_ < other.last_); }
    bool operator==(const Range<T>& other) const { return first_ == other.first_ && last_ == other.last_; }
    bool operator>=(const Range<T>& other) const { return *this > other || *this == other; }
    bool operator<=(const Range<T>& other) const { return *this < other || *this == other; }

private:
    const T first_;
    const T last_;
};

///
/// Helpers
///
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
