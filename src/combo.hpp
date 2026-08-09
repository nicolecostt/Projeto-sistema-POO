#ifndef COMBO_HPP
#define COMBO_HPP

#include "counted.hpp"
#include "product.hpp"
#include <vector>
#include <functional>
#include <string>

class combo final : public product, public counted<combo> {
private:
    std::vector<std::reference_wrapper<const product>> itens_;
    double porcentagem_desconto_;

public:
    combo(const std::string& name, double porcentagem_desconto);
    ~combo() override;

    void adicionar_produto(const product& p);
    double calcular_calorias() const override;
    void exibir_info() const override;
    std::string type_name() const override;
    double get_price() const override;

    double get_discount_percent() const;
    const std::vector<std::reference_wrapper<const product>>& get_items() const;
};

#endif
