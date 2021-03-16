#ifndef UTILS_H
#define UTILS_H

#include "Range.h"

#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <functional>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <assert.h>

// TODO make a Util sub-folder and have these as seperate classes, the single file thing is just laziness!
namespace Tril {

///
/// Constants
///
constexpr double Pi = 3.141592653589793;
constexpr double Tau = Pi * 2;

///
/// Types
///

template <typename T>
class MinMax {
public:
    MinMax()
        : min_(std::numeric_limits<T>::max())
        , max_(std::numeric_limits<T>::lowest())
    {}
    MinMax(const T& a, const T& b)
        : min_(std::min(a, b))
        , max_(std::max(a, b))
    {}

    bool IsValid() const { return min_ <= max_; }
    T Min() const { return IsValid() ? min_ : T{ 0 }; }
    T Max() const { return IsValid() ? max_ : T{ 0 }; }
    T Range() const { return IsValid() ? max_ - min_ : T{ 0 }; }
    bool Contains(const T& value) const { return value >= min_ && value <= max_; }
    operator Tril::Range<T>() const { return Tril::Range(Min(), Max()); }

    void ExpandToContain(const T& newValue)
    {
        if (IsValid()) {
            if (newValue < min_) {
                min_ = newValue;
            }
            if (newValue > max_) {
                max_ = newValue;
            }
        } else {
            max_ = newValue;
            min_ = newValue;
        }
    }
    void SetMin(const T& newMin)
    {
        min_ = newMin;
        max_ = std::max(max_, newMin);
    }
    void SetMax(const T& newMax)
    {
        min_ = std::min(min_, newMax);
        max_ = newMax;
    }
    void SetRange(const T& a, const T& b)
    {
        min_ = std::min(a, b);
        max_ = std::max(a, b);
    }
    void Reset()
    {
        min_ = std::numeric_limits<T>::max();
        max_ = std::numeric_limits<T>::lowest();
    }

private:
    T min_;
    T max_;
};

class RollingStatistics {
public:
    void AddValue(const double& value);

    uint64_t Count() const { return count_; }
    double Mean() const { return sumOfValues_ / count_; }
    double StandardDeviation() const;
    double Min() const { return min_; };
    double Max() const { return max_; };

private:
    uint64_t count_ = 0;
    double sumOfValues_ = 0.0;
    double sumOfValuesSquared_ = 0.0;
    double min_ = std::numeric_limits<double>::max();
    double max_ = std::numeric_limits<double>::min();
};

template <typename T>
class CircularBuffer {
public:
    CircularBuffer(size_t capacity)
        : items_(capacity, T{})
        , next_(0)
        , fill_(0)
    {
        items_.shrink_to_fit();
    }

    size_t Size() const { return fill_; }
    size_t Capacity() const { return items_.size(); }
    bool Full() const { return Size() == Capacity(); }
    bool Empty() const { return Size() == 0; }

    void ForEach(const std::function<void(const T& item)>& action) const
    {
        auto iter = items_.cbegin();
        size_t startIndex = fill_ == items_.size() ? next_ : 0;
        std::advance(iter, startIndex);
        for (size_t count = 0; count < fill_; ++count) {
            action(*iter++);
            if (iter == items_.cend()) {
                iter = items_.cbegin();
            }
        }
    }

    void Resize(size_t size)
    {
        CircularBuffer copy(size);
        ForEach([&](const T& item)
        {
            copy.PushBack(item);
        });
        items_ = std::move(copy.items_);
        next_ = copy.next_;
        fill_ = copy.fill_;
    }

    void PushBack(const T& item)
    {
        if (items_.size() > 0) {
            items_[next_++] = item;
            if (next_ == items_.size()) {
                next_ = 0;
            }
            if (fill_ < items_.size()) {
                ++fill_;
            }
        }
    }

    void Clear()
    {
        std::fill(std::begin(items_), std::end(items_), T{});
        next_ = 0;
        fill_ = 0;
    }

private:
    std::vector<T> items_;
    size_t next_;
    size_t fill_;
};

/**
 * A container that provides a handle per item contained, and lazily removes the
 * item once no remaining instances of the handle exist.
 */
using Handle = std::shared_ptr<int>;

template <typename ValueType>
class AutoClearingContainer {
public:

    using value_type = ValueType;

    [[nodiscard]] Handle PushBack(ValueType&& value)
    {
        auto lifetime = std::make_shared<int>(0);
        values_.insert({lifetime, std::move(value)});
        return lifetime;
    }

    void ForEach(const std::function<void(ValueType&)>& action)
    {
        // TODO use c++20 std::erase_if when possible
        for(auto iter = std::begin(values_); iter != std::end(values_); ) {
            auto& [handle, value] = *iter;
             if (auto exists = handle.lock()) {
                 action(value);
                 ++iter;
             } else {
                 iter = values_.erase(iter);
             }
        }
    }

private:
    std::map<std::weak_ptr<int>, ValueType, std::owner_less<std::weak_ptr<int>>> values_;
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
    prefix = "std::string_view Tril::TypeName() [T = ";
    suffix = "]";
#elif defined(__GNUC__)
    name = __PRETTY_FUNCTION__;
    prefix = "constexpr std::string_view Tril::TypeName() [with T = ";
    suffix = "; std::string_view = std::basic_string_view<char>]";
#elif defined(_MSC_VER)
    name = __FUNCSIG__;
    prefix = "class std::basic_string_view<char,struct std::char_traits<char> > __cdecl Tril::TypeName<";
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

} // namespace Tril

#endif // UTILS_H
