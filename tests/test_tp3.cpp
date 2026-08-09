#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "beverage.hpp"
#include "domain_errors.hpp"
#include "food.hpp"
#include "lanchonete_app.hpp"
#include "menu_catalog.hpp"
#include "product_concepts.hpp"
#include "registry.hpp"
#include "serialization.hpp"

#include <memory>
#include <string>
#include <type_traits>
#include <vector>

using Catch::Approx;

TEST_CASE("registry and priced_item concept support generic programming", "[tp3][generic]") {
    registry<std::string> tags;
    tags.add("lanche");
    tags.add("bebida");

    registry<int> priorities;
    priorities.add(1);
    priorities.add(2);

    REQUIRE(tags.size() == 2);
    REQUIRE(tags.at(0) == "lanche");
    REQUIRE(priorities.at(1) == 2);

    static_assert(priced_item<product>);
    static_assert(priced_item<alimento>);
    static_assert(priced_item<bebida>);
    static_assert(!priced_item<int>);

    std::vector<alimento> foods;
    foods.emplace_back("Burger", 20.0, 600.0, "lanche");
    foods.emplace_back("Batata", 8.0, 400.0, "acompanhamento");

    REQUIRE(calculate_price_sum(foods) == Approx(28.0));
}

TEST_CASE("domain errors and optional search expose clear failure paths", "[tp3][errors]") {
    alimento burger("Burger", 20.0, 600.0, "lanche");
    menu_catalog catalog;

    catalog.add_product(burger, "lanche");

    auto found = catalog.find_by_name("Burger");
    auto missing = catalog.find_by_name("Suco");

    REQUIRE(found.has_value());
    REQUIRE(found->get().get_name() == "Burger");
    REQUIRE_FALSE(missing.has_value());

    REQUIRE_THROWS_AS(catalog.add_product(burger, ""), domain_error);
    REQUIRE_THROWS_AS(catalog.add_product(burger, "lanche"), domain_error);
}

TEST_CASE("serialization round-trip preserves app state and product variants", "[tp3][json]") {
    app_state original;
    original.products.push_back(food_record{"Burger", 20.0, 600.0, "lanche"});
    original.products.push_back(beverage_record{"Suco", 7.0, 300, false});
    original.products.push_back(combo_record{"Combo", 10.0, {"Burger", "Suco"}});

    nlohmann::json doc = original;
    app_state loaded = doc.get<app_state>();

    REQUIRE(loaded.version == 1);
    REQUIRE(loaded.products.size() == 3);
    REQUIRE(std::holds_alternative<food_record>(loaded.products.at(0)));
    REQUIRE(std::holds_alternative<beverage_record>(loaded.products.at(1)));
    REQUIRE(std::holds_alternative<combo_record>(loaded.products.at(2)));
    REQUIRE(doc.at("version").get<int>() == 1);
    REQUIRE(doc.at("products").at(0).at("type").get<std::string>() == "food");
}

TEST_CASE("lanchonete_app uses memory_repository through DIP without files", "[tp3][dip]") {
    app_state initial;
    initial.products.push_back(food_record{"Burger", 20.0, 600.0, "lanche"});

    memory_repository repo;
    lanchonete_app app{repo};

    app.replace_state(initial);
    app.save();

    app_state loaded = app.load();
    REQUIRE(loaded.products.size() == 1);
    REQUIRE(std::holds_alternative<food_record>(loaded.products.front()));
}

TEST_CASE("lanchonete_app exposes thin operations for the Qt GUI", "[tp3][qt]") {
    memory_repository repo;
    lanchonete_app app{repo};

    app.add_food_record("Burger", 20.0, 600.0, "lanche");
    app.add_food_record("Batata", 8.0, 400.0, "acompanhamento");

    REQUIRE(app.current_state().products.size() == 2);
    REQUIRE(app.total_product_price() == Approx(28.0));

    app.remove_product_at(0);

    REQUIRE(app.current_state().products.size() == 1);
    REQUIRE(app.total_product_price() == Approx(8.0));
    REQUIRE_THROWS_AS(app.remove_product_at(10), domain_error);
}

TEST_CASE("parallel calories calculation matches serial result", "[tp3][stl][concurrency]") {
    std::vector<std::unique_ptr<product>> products;
    products.push_back(std::make_unique<alimento>("Burger", 20.0, 600.0, "lanche"));
    products.push_back(std::make_unique<bebida>("Refrigerante", 5.0, 350, true));
    products.push_back(std::make_unique<alimento>("Batata", 8.0, 400.0, "acompanhamento"));

    const double serial = calculate_total_calories_serial(products);
    const double parallel = calculate_total_calories_parallel(products);

    REQUIRE(serial == Approx(1147.0));
    REQUIRE(parallel == Approx(serial));
}
