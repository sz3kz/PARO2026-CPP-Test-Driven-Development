#pragma once
#include <string>
#include <vector>

struct Product
{
    long id;
    std::string name;
    double price;
};

using Registry = std::vector<Product>;

void addProduct(Registry& registry, struct Product const& product);
