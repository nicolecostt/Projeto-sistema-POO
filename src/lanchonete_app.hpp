#ifndef LANCHONETE_APP_HPP
#define LANCHONETE_APP_HPP

#include "repository.hpp"

#include <cstddef>
#include <string>

class lanchonete_app {
private:
    state_repository& repository_;
    app_state state_;

public:
    explicit lanchonete_app(state_repository& repository);

    void replace_state(const app_state& state);
    const app_state& current_state() const;

    void add_food_record(const std::string& name, double price,
                         double calories, const std::string& category);
    void remove_product_at(std::size_t index);
    double total_product_price() const;

    void save() const;
    app_state load();
};

#endif
