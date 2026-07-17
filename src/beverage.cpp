#include "beverage.hpp"
#include <iostream>

bebida::bebida(const std::string& name, double price, int volume_ml, 
               bool tem_gas)
    : product(name, price), volume_ml_(volume_ml), tem_gas_(tem_gas) {
    std::cout << "Bebida \"" << name_ << "\" criada\n";
}

bebida::~bebida() {
    std::cout << "~Bebida \"" << name_ << "\" destruida\n";
}

double bebida::calcular_calorias() const {
    if (tem_gas_) {
        return volume_ml_ * 0.42;
    } else {
        return volume_ml_ * 0.20;
    }
}

void bebida::exibir_info() const {
    product::exibir_info();
    std::cout << " | Volume: " << volume_ml_ << "ml"
              << " | " << (tem_gas_ ? "Com gas" : "Sem gas")
              << " | Calorias: " << calcular_calorias() << " kcal\n";
}

double bebida::calcular_preco_promocional(double porcentagem_desconto) const {
    return apply_discount(porcentagem_desconto);
}

bool bebida::elegivel_para_promocao() const {
    return true;
}
