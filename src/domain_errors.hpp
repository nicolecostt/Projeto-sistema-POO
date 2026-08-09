#ifndef DOMAIN_ERRORS_HPP
#define DOMAIN_ERRORS_HPP

#include <stdexcept>
#include <string>

class domain_error : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class invalid_quantity_error : public domain_error {
public:
    explicit invalid_quantity_error(const std::string& message)
        : domain_error("invalid quantity: " + message) {}
};

class invalid_product_error : public domain_error {
public:
    explicit invalid_product_error(const std::string& message)
        : domain_error("invalid product: " + message) {}
};

class repository_error : public domain_error {
public:
    explicit repository_error(const std::string& message)
        : domain_error("repository error: " + message) {}
};

#endif
