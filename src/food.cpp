#include "food.hpp"
#include <iostream>

alimento::alimento(const std::string& name, double price, double calorias, 
                   const std::string& categoria)
    : product(name, price), calorias_(calorias), categoria_(categoria) {
    std::cout << "Alimento \"" << name_ << "\" (" << categoria_ << ") criado\n";
}

alimento::~alimento() {
    std::cout << "~Alimento \"" << name_ << "\" destruido\n";
}

double alimento::calcular_calorias() const {
    return calorias_;
}

void alimento::exibir_info() const {
    product::exibir_info();
    std::cout << " | Categoria: " << categoria_ 
              << " | Calorias: " << calorias_ << " kcal\n";
}

std::string alimento::type_name() const {
    return "food";
}

double alimento::get_calories() const {
    return calorias_;
}

std::string alimento::get_category() const {
    return categoria_;
}

double alimento::calcular_preco_promocional(double porcentagem_desconto) const {
    return apply_discount(porcentagem_desconto);
}

bool alimento::elegivel_para_promocao() const {
    return true;
}
