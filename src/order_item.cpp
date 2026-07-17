#include "order_item.hpp"

#include <iostream>

order_item::order_item(const product& product_ref, int quantity)
    : product_(product_ref), quantity_(quantity) {
    std::cout << "Item \"" << product_.get_name() << "\" criado\n";
}

order_item::~order_item() {
    std::cout << "~Item \"" << product_.get_name() << "\" destruido\n";
}

const product& order_item::get_product() const {
    return product_;
}

int order_item::get_quantity() const {
    return quantity_;
}

double order_item::calculate_subtotal() const {
    if (quantity_ <= 0) {
        return 0.0;
    }

    return product_.get_price() * quantity_;
}
