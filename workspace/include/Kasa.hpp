#pragma once
#include <map>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

struct Product
{
    long identifier{ 0 };
    std::string name;
    double price{ 0 };

    Product() = default;

    Product(long product_identifier,
            std::string product_name,
            double product_price)
      : identifier(product_identifier)
      , name(std::move(product_name))
      , price(product_price) {};
};

using Registry = std::map<long, Product>;
using Cart = std::vector<long>;

void registerProduct(Registry& registry, struct Product const& product);
void deregisterProduct(Registry& registry, long identifier);
void cartAddProduct(Registry const& registry, Cart& cart, long identifier);
void cartDeleteProduct(Cart& cart, long identifier);
auto calculateCartValue(Registry const& registry, Cart const& cart) -> double;
void cartClose(Cart& cart);
