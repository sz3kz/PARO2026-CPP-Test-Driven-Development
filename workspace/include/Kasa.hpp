#pragma once
#include <string>
#include <vector>

struct Product
{
    long id{ 0 };
    std::string name{ "" };
    double price{ 0 };

    Product() = default;

    Product(long product_id, std::string product_name, double product_price)
      : id(product_id)
      , name(product_name)
      , price(product_price) {};
};

using Registry = std::vector<Product>;
using Cart = std::vector<double>;

void registerProduct(Registry& registry, struct Product const& product);
void deregisterProduct(Registry& registry, double id);
void cartAddProduct(Registry const& registry, Cart& cart, double id);
