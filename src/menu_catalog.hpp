#ifndef MENU_CATALOG_HPP
#define MENU_CATALOG_HPP

#include "product.hpp"

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

class menu_catalog {
private:
    std::map<std::string, std::reference_wrapper<const product>> products_by_name_;
    std::unordered_set<std::string> categories_;

public:
    void add_product(const product& item, const std::string& category);

    std::optional<std::reference_wrapper<const product>>
    find_by_name(const std::string& name) const;

    std::vector<const product*> products_sorted_by_price() const;
    std::vector<std::string> product_names() const;
    std::vector<std::string> expensive_product_names(double minimum_price) const;

    std::size_t count_products_above(double minimum_price) const;
    double total_price() const;

    const std::unordered_set<std::string>& categories() const;
};

double calculate_total_calories_serial(
    const std::vector<std::unique_ptr<product>>& products);

double calculate_total_calories_parallel(
    const std::vector<std::unique_ptr<product>>& products);

#endif
