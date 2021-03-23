#ifndef AUTOCLEARINGCONTAINER_H
#define AUTOCLEARINGCONTAINER_H

#include <memory>
#include <map>
#include <functional>

namespace Tril {

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

} // end namespace Tril

#endif // AUTOCLEARINGCONTAINER_H
