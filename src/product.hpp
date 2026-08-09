#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include <string>
#include <memory>
#include <vector>

class product {
protected:
    std::string name_;
    double price_;

public:
    product(const std::string& name, double price);
    virtual ~product();

    virtual double calcular_calorias() const = 0;
    virtual void exibir_info() const;
    virtual std::string type_name() const = 0;

    std::string get_name() const;
    virtual double get_price() const;

    double apply_discount(double percent) const;
};

const product* encontrar_maior_calorias(
    const std::vector<std::unique_ptr<product>>& products);

#endif
