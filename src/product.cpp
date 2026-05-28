#include "product.hpp"

#include <iostream>

product::product(const std::string& name, double price)
    : name_(name), price_(price) {
    std::cout << "Produto \"" << name_ << "\" criado\n";
}

product::~product() {
    std::cout << "~Produto \"" << name_ << "\" destruido\n";
}

std::string product::get_name() const {
    return name_;
}

double product::get_price() const {
    return price_;
}

double product::apply_discount(double percent) const {
    if (percent < 0) {
        percent = 0;
    }

    if (percent > 100) {
        percent = 100;
    }

    return price_ - (price_ * percent / 100.0);
}
