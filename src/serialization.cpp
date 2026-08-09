#include "serialization.hpp"

#include "domain_errors.hpp"

#include <string>

void to_json(nlohmann::json& j, const food_record& record) {
    j = nlohmann::json{
        {"type", "food"},
        {"name", record.name},
        {"price", record.price},
        {"calories", record.calories},
        {"category", record.category},
    };
}

void from_json(const nlohmann::json& j, food_record& record) {
    record = food_record{
        j.at("name").get<std::string>(),
        j.at("price").get<double>(),
        j.at("calories").get<double>(),
        j.at("category").get<std::string>(),
    };
}

void to_json(nlohmann::json& j, const beverage_record& record) {
    j = nlohmann::json{
        {"type", "beverage"},
        {"name", record.name},
        {"price", record.price},
        {"volume_ml", record.volume_ml},
        {"has_gas", record.has_gas},
    };
}

void from_json(const nlohmann::json& j, beverage_record& record) {
    record = beverage_record{
        j.at("name").get<std::string>(),
        j.at("price").get<double>(),
        j.at("volume_ml").get<int>(),
        j.at("has_gas").get<bool>(),
    };
}

void to_json(nlohmann::json& j, const combo_record& record) {
    j = nlohmann::json{
        {"type", "combo"},
        {"name", record.name},
        {"discount_percent", record.discount_percent},
        {"item_names", record.item_names},
    };
}

void from_json(const nlohmann::json& j, combo_record& record) {
    record = combo_record{
        j.at("name").get<std::string>(),
        j.at("discount_percent").get<double>(),
        j.at("item_names").get<std::vector<std::string>>(),
    };
}

void to_json(nlohmann::json& j, const product_record& record) {
    std::visit([&j](const auto& concrete_record) {
        to_json(j, concrete_record);
    }, record);
}

void from_json(const nlohmann::json& j, product_record& record) {
    const auto type = j.at("type").get<std::string>();

    if (type == "food") {
        record = j.get<food_record>();
    } else if (type == "beverage") {
        record = j.get<beverage_record>();
    } else if (type == "combo") {
        record = j.get<combo_record>();
    } else {
        throw invalid_product_error("unknown serialized type: " + type);
    }
}

void to_json(nlohmann::json& j, const app_state& state) {
    j = nlohmann::json{
        {"version", state.version},
        {"products", state.products},
    };
}

void from_json(const nlohmann::json& j, app_state& state) {
    const int version = j.at("version").get<int>();
    if (version != 1) {
        throw repository_error("unsupported state version: " + std::to_string(version));
    }

    state.version = version;
    state.products = j.at("products").get<std::vector<product_record>>();
}
