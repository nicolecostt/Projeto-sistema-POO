#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include <string>

class product {
private:
    std::string name_;
    double price_;

public:
    product(const std::string& name, double price);
    ~product();

    std::string get_name() const;
    double get_price() const;

    double apply_discount(double percent) const;
};

#endif
