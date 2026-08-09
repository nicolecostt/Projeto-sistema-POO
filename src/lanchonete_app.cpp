#include "lanchonete_app.hpp"

#include "domain_errors.hpp"

#include <cstddef>
#include <numeric>
#include <string>
#include <variant>

lanchonete_app::lanchonete_app(state_repository& repository)
    : repository_(repository) {}

void lanchonete_app::replace_state(const app_state& state) {
    state_ = state;
}

const app_state& lanchonete_app::current_state() const {
    return state_;
}

void lanchonete_app::add_food_record(const std::string& name, double price,
                                     double calories,
                                     const std::string& category) {
    if (name.empty()) {
        throw invalid_product_error("food name must not be empty");
    }
    if (category.empty()) {
        throw invalid_product_error("food category must not be empty");
    }
    if (price < 0.0) {
        throw invalid_product_error("food price must not be negative");
    }
    if (calories < 0.0) {
        throw invalid_product_error("food calories must not be negative");
    }

    state_.products.push_back(food_record{name, price, calories, category});
}

void lanchonete_app::remove_product_at(std::size_t index) {
    if (index >= state_.products.size()) {
        throw invalid_product_error("product index is out of range");
    }

    state_.products.erase(state_.products.begin() + static_cast<std::ptrdiff_t>(index));
}

double lanchonete_app::total_product_price() const {
    return std::accumulate(
        state_.products.begin(), state_.products.end(), 0.0,
        [](double total, const product_record& record) {
            return total + std::visit([](const auto& concrete_record) {
                if constexpr (requires { concrete_record.price; }) {
                    return concrete_record.price;
                } else {
                    return 0.0;
                }
            }, record);
        });
}

void lanchonete_app::save() const {
    repository_.save(state_);
}

app_state lanchonete_app::load() {
    state_ = repository_.load();
    return state_;
}
