#ifndef SERIALIZATION_HPP
#define SERIALIZATION_HPP

#include <nlohmann/json.hpp>

#include <string>
#include <variant>
#include <vector>

struct food_record {
    std::string name;
    double price;
    double calories;
    std::string category;

    bool operator==(const food_record&) const = default;
};

struct beverage_record {
    std::string name;
    double price;
    int volume_ml;
    bool has_gas;

    bool operator==(const beverage_record&) const = default;
};

struct combo_record {
    std::string name;
    double discount_percent;
    std::vector<std::string> item_names;

    bool operator==(const combo_record&) const = default;
};

using product_record = std::variant<food_record, beverage_record, combo_record>;

struct app_state {
    int version = 1;
    std::vector<product_record> products;

    bool operator==(const app_state&) const = default;
};

void to_json(nlohmann::json& j, const food_record& record);
void from_json(const nlohmann::json& j, food_record& record);

void to_json(nlohmann::json& j, const beverage_record& record);
void from_json(const nlohmann::json& j, beverage_record& record);

void to_json(nlohmann::json& j, const combo_record& record);
void from_json(const nlohmann::json& j, combo_record& record);

void to_json(nlohmann::json& j, const product_record& record);
void from_json(const nlohmann::json& j, product_record& record);

void to_json(nlohmann::json& j, const app_state& state);
void from_json(const nlohmann::json& j, app_state& state);

#endif
