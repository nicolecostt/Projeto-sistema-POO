#include "product.hpp"

#include <iostream>

product::product(const std::string& name, double price)
    : name_(name), price_(price) {
    std::cout << "Produto \"" << name_ << "\" criado\n";
}

product::~product() {
    std::cout << "~Produto \"" << name_ << "\" destruido\n";
}

void product::exibir_info() const {
    std::cout << "Produto: " << name_ << " - R$ " << price_;
}

std::string product::get_name() const {
    return name_;
}

double product::get_price() const {
    return price_;
}

double product::apply_discount(double percent) const {
    if (percent < 0) {
        percent = 0;
    }

    if (percent > 100) {
        percent = 100;
    }

    return price_ - (price_ * percent / 100.0);
}

const product* encontrar_maior_calorias(
    const std::vector<std::unique_ptr<product>>& products) {
    if (products.empty()) return nullptr;
    
    const product* maior = products[0].get();
    for (const auto& p : products) {
        if (p->calcular_calorias() > maior->calcular_calorias()) {
            maior = p.get();
        }
    }
    return maior;
}
