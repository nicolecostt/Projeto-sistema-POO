#include "customer.hpp"

#include <iostream>

customer::customer(const std::string& name, const std::string& cpf)
    : name_(name), cpf_(cpf) {
    std::cout << "Cliente \"" << name_ << "\" criado\n";
}

customer::~customer() {
    std::cout << "~Cliente \"" << name_ << "\" destruido\n";
}

std::string customer::get_name() const {
    return name_;
}

std::string customer::get_cpf() const {
    return cpf_;
}

bool customer::is_valid_cpf() const {
    return cpf_.size() == 11;
}
