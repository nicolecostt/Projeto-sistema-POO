#include "order.hpp"

#include <iomanip>
#include <iostream>

order::order(int id, const customer& customer_ref)
    : id_(id), customer_(customer_ref) {
    std::cout << "Pedido " << id_ << " criado\n";
}

order::~order() {
    std::cout << "~Pedido " << id_ << " destruido\n";
}

int order::get_id() const {
    return id_;
}

const customer& order::get_customer() const {
    return customer_;
}

void order::add_item(const product& product_ref, int quantity) {
    if (quantity <= 0) {
        std::cout << "Quantidade invalida para o produto \""
                  << product_ref.get_name() << "\"\n";
        return;
    }

    items_.push_back(std::make_unique<order_item>(product_ref, quantity));
}

double order::calculate_total() const {
    double total = 0.0;

    for (const auto& item : items_) {
        total += item->calculate_subtotal();
    }

    return total;
}

void order::print_summary() const {
    std::cout << "\nResumo do pedido " << id_ << "\n";
    std::cout << "Cliente: " << customer_.get_name() << "\n";

    for (const auto& item : items_) {
        std::cout << "- " << item->get_product().get_name()
                  << " x" << item->get_quantity()
                  << " = R$ " << std::fixed << std::setprecision(2)
                  << item->calculate_subtotal() << "\n";
    }

    std::cout << "Total: R$ " << std::fixed << std::setprecision(2)
              << calculate_total() << "\n\n";
}
