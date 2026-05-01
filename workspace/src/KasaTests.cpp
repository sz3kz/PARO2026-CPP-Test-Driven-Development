#include "Kasa.hpp"
#include <gtest/gtest.h>
#include <random>

TEST(
  KasaTests,
  ProductAddition_RandomizedProductCount_RegistrySizeMatchesGeneratedProductCount)
{
    Registry registry;
    std::random_device my_random_device;
    std::mt19937 my_generator(my_random_device());
    std::uniform_int_distribution<> distr(1, 100);
    int random_product_count = distr(my_generator);

    for (int i = 0; i < random_product_count; ++i)
    {
        struct Product temp_product;
        temp_product.id = i + 10;
        temp_product.name = "Product";
        temp_product.price = 10.00;
        addProduct(registry, temp_product);
    }
    EXPECT_EQ(registry.size(), random_product_count);
}
