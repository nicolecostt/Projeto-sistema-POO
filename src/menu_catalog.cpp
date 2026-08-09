#include "menu_catalog.hpp"

#include "domain_errors.hpp"

#include <algorithm>
#include <future>
#include <iterator>
#include <mutex>
#include <numeric>
#include <ranges>

void menu_catalog::add_product(const product& item, const std::string& category) {
    if (category.empty()) {
        throw invalid_product_error("category must not be empty");
    }

    const auto result = products_by_name_.emplace(item.get_name(), std::cref(item));
    if (!result.second) {
        throw invalid_product_error("duplicated name: " + item.get_name());
    }

    categories_.insert(category);
}

std::optional<std::reference_wrapper<const product>>
menu_catalog::find_by_name(const std::string& name) const {
    const auto found = products_by_name_.find(name);
    if (found == products_by_name_.end()) {
        return std::nullopt;
    }
    return found->second;
}

std::vector<const product*> menu_catalog::products_sorted_by_price() const {
    std::vector<const product*> products;
    products.reserve(products_by_name_.size());

    std::transform(products_by_name_.begin(), products_by_name_.end(),
                   std::back_inserter(products),
                   [](const auto& entry) { return &entry.second.get(); });

    std::sort(products.begin(), products.end(),
              [](const product* left, const product* right) {
                  return left->get_price() < right->get_price();
              });

    return products;
}

std::vector<std::string> menu_catalog::product_names() const {
    std::vector<std::string> names;
    names.reserve(products_by_name_.size());

    std::transform(products_by_name_.begin(), products_by_name_.end(),
                   std::back_inserter(names),
                   [](const auto& entry) { return entry.first; });

    return names;
}

std::vector<std::string>
menu_catalog::expensive_product_names(double minimum_price) const {
    namespace rv = std::views;

    auto expensive_names = products_by_name_
        | rv::values
        | rv::filter([minimum_price](const auto& item) {
              return item.get().get_price() >= minimum_price;
          })
        | rv::transform([](const auto& item) {
              return item.get().get_name();
          });

    std::vector<std::string> names;
    for (const auto& name : expensive_names) {
        names.push_back(name);
    }
    return names;
}

std::size_t menu_catalog::count_products_above(double minimum_price) const {
    return static_cast<std::size_t>(
        std::count_if(products_by_name_.begin(), products_by_name_.end(),
                      [minimum_price](const auto& entry) {
                          return entry.second.get().get_price() > minimum_price;
                      }));
}

double menu_catalog::total_price() const {
    return std::accumulate(products_by_name_.begin(), products_by_name_.end(), 0.0,
                           [](double total, const auto& entry) {
                               return total + entry.second.get().get_price();
                           });
}

const std::unordered_set<std::string>& menu_catalog::categories() const {
    return categories_;
}

double calculate_total_calories_serial(
    const std::vector<std::unique_ptr<product>>& products) {
    return std::accumulate(products.begin(), products.end(), 0.0,
                           [](double total, const auto& item) {
                               return total + item->calcular_calorias();
                           });
}

double calculate_total_calories_parallel(
    const std::vector<std::unique_ptr<product>>& products) {
    std::vector<std::future<double>> futures;
    futures.reserve(products.size());

    for (const auto& item : products) {
        const product* raw_item = item.get();
        futures.push_back(std::async(std::launch::async, [raw_item] {
            return raw_item->calcular_calorias();
        }));
    }

    std::mutex mutex;
    double total = 0.0;
    for (auto& future : futures) {
        const double partial = future.get();
        std::lock_guard<std::mutex> lock{mutex};
        total += partial;
    }

    return total;
}
