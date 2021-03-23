#ifndef FORMATHELPERS_H
#define FORMATHELPERS_H

#include <fmt/core.h>
#include <fmt/format.h>

// // This is way too ambiguous until I can use C++20's type traits... Then I can make a generic Container formatter
// template<typename Container>
// struct fmt::formatter<Container> : fmt::formatter<typename Container::value_type>
// {
//     template <typename FormatContext>
//     auto format(const Container& container, FormatContext& context)
//     {
//         auto&& out= context.out();
//         format_to(out, "{{");
//
//         bool first = true;
//         for (const auto& item : container) {
//             if (!first) {
//                 first = false;
//             } else {
//                 format_to(out, ", ");
//             }
//             fmt::formatter<typename Container::value_type>::format(item, context);
//         }
//
//         return format_to(out, "}}");
//     }
// };

template<typename ValueType>
struct fmt::formatter<std::vector<ValueType>> : fmt::formatter<ValueType>
{
    template <typename FormatContext>
    auto format(const std::vector<ValueType>& container, FormatContext& context)
    {
        auto&& out= context.out();
        format_to(out, "{{");

        bool first = true;
        for (const auto& item : container) {
            if (!first) {
                first = false;
            } else {
                format_to(out, ", ");
            }
            fmt::formatter<ValueType>::format(item, context);
        }

        return format_to(out, "}}");
    }
};

#endif // FORMATHELPERS_H
