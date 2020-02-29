#ifndef UTILS_H
#define UTILS_H

#include <string_view>

namespace EoBE {

///
/// Constants
///
constexpr double Pi = 3.141592653589793;
constexpr double Tau = Pi * 2;


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

} // namespace EoBE

#endif // UTILS_H
