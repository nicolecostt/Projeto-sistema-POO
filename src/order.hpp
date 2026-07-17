#ifndef ORDER_HPP
#define ORDER_HPP

#include "customer.hpp"
#include "order_item.hpp"
#include "product.hpp"

#include <memory>
#include <vector>

class order {
private:
    int id_;
    const customer& customer_;
    std::vector<std::unique_ptr<order_item>> items_;

public:
    order(int id, const customer& customer_ref);
    ~order();

    int get_id() const;
    const customer& get_customer() const;

    void add_item(const product& product_ref, int quantity);
    double calculate_total() const;
    void print_summary() const;
};

#endif
