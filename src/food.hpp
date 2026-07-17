#ifndef FOOD_HPP
#define FOOD_HPP

#include "product.hpp"
#include "promotable.hpp"
#include <string>

class alimento : public product, public promocionavel {
private:
    double calorias_;
    std::string categoria_;

public:
    alimento(const std::string& name, double price, double calorias, 
             const std::string& categoria);
    ~alimento() override;

    double calcular_calorias() const override;
    void exibir_info() const override;

    double calcular_preco_promocional(double porcentagem_desconto) const override;
    bool elegivel_para_promocao() const override;
};

#endif
