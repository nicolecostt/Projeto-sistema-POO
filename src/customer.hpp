#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include <string>

class customer {
private:
    std::string name_;
    std::string cpf_;

public:
    customer(const std::string& name, const std::string& cpf);
    ~customer();

    std::string get_name() const;
    std::string get_cpf() const;

    bool is_valid_cpf() const;
};

#endif
