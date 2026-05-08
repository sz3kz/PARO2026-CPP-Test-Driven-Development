#pragma once
#include "Product.hpp"
#include <iostream>
#include <string>

struct Product
{
    long identifier;
    std::string name;
    double price;

    explicit Product(long product_identifier,
                     std::string product_name,
                     double product_price)
      : identifier(product_identifier)
      , name(std::move(product_name))
      , price(product_price) {};
    friend auto operator<<(std::ostream& output_stream,
                           struct Product const& product) -> std::ostream&;
};
