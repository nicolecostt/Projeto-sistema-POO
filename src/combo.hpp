#ifndef COMBO_HPP
#define COMBO_HPP

#include "product.hpp"
#include <vector>
#include <functional>
#include <string>

class combo final : public product {
private:
    std::vector<std::reference_wrapper<const product>> itens_;
    double porcentagem_desconto_;

public:
    combo(const std::string& name, double porcentagem_desconto);
    ~combo() override;

    void adicionar_produto(const product& p);
    double calcular_calorias() const override;
    void exibir_info() const override;
    double get_price() const override;
};

#endif
