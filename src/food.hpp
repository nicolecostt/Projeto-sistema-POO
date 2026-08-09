#ifndef FOOD_HPP
#define FOOD_HPP

#include "counted.hpp"
#include "product.hpp"
#include "promotable.hpp"
#include <string>

class alimento : public product, public promocionavel, public counted<alimento> {
private:
    double calorias_;
    std::string categoria_;

public:
    alimento(const std::string& name, double price, double calorias, 
             const std::string& categoria);
    ~alimento() override;

    double calcular_calorias() const override;
    void exibir_info() const override;
    std::string type_name() const override;

    double get_calories() const;
    std::string get_category() const;

    double calcular_preco_promocional(double porcentagem_desconto) const override;
    bool elegivel_para_promocao() const override;
};

#endif
