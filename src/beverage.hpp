#ifndef BEVERAGE_HPP
#define BEVERAGE_HPP

#include "product.hpp"
#include "promotable.hpp"
#include <string>

class bebida : public product, public promocionavel {
private:
    int volume_ml_;
    bool tem_gas_;

public:
    bebida(const std::string& name, double price, int volume_ml, 
           bool tem_gas);
    ~bebida() override;

    double calcular_calorias() const override final;
    void exibir_info() const override;

    double calcular_preco_promocional(double porcentagem_desconto) const override;
    bool elegivel_para_promocao() const override;
};

#endif
