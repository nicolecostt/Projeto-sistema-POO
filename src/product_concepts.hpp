#ifndef PRODUCT_CONCEPTS_HPP
#define PRODUCT_CONCEPTS_HPP

#include <concepts>
#include <vector>

template <typename T>
concept priced_item = requires(const T& item) {
    { item.get_price() } -> std::convertible_to<double>;
};

template <priced_item T>
double calculate_price_sum(const std::vector<T>& items) {
    double total = 0.0;
    for (const auto& item : items) {
        total += item.get_price();
    }
    return total;
}

#endif
