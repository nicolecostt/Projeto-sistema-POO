#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include <cstddef>
#include <utility>
#include <vector>

template <typename T>
class registry {
private:
    std::vector<T> items_;

public:
    void add(T item) {
        items_.push_back(std::move(item));
    }

    const T& at(std::size_t index) const {
        return items_.at(index);
    }

    std::size_t size() const {
        return items_.size();
    }
};

#endif
