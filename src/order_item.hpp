#ifndef ORDER_ITEM_HPP
#define ORDER_ITEM_HPP

#include "product.hpp"

class order_item {
private:
    const product& product_;
    int quantity_;

public:
    order_item(const product& product_ref, int quantity);
    ~order_item();

    const product& get_product() const;
    int get_quantity() const;

    double calculate_subtotal() const;
};

#endif
