#include "combo.hpp"
#include <iostream>

combo::combo(const std::string& name, double porcentagem_desconto)
    : product(name, 0.0), porcentagem_desconto_(porcentagem_desconto) {
    std::cout << "Combo \"" << name_ << "\" criado\n";
}

combo::~combo() {
    std::cout << "~Combo \"" << name_ << "\" destruido\n";
}

void combo::adicionar_produto(const product& p) {
    itens_.push_back(std::cref(p));
    price_ += p.get_price();
}

double combo::calcular_calorias() const {
    double total_calorias = 0.0;
    for (const auto& item : itens_) {
        total_calorias += item.get().calcular_calorias();
    }
    return total_calorias;
}

void combo::exibir_info() const {
    std::cout << "Combo: " << name_ << "\n";
    std::cout << "  Itens do combo:\n";
    for (const auto& item : itens_) {
        std::cout << "    - " << item.get().get_name() 
                  << " (R$ " << item.get().get_price() << ")\n";
    }
    std::cout << "  Preco total: R$ " << get_price() 
              << " (desconto de " << porcentagem_desconto_ << "%)\n";
    std::cout << "  Calorias totais: " << calcular_calorias() << " kcal\n";
}

double combo::get_price() const {
    return price_ - (price_ * porcentagem_desconto_ / 100.0);
}
