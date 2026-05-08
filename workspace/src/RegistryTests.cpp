#include "Kasa.hpp"
#include "Product.hpp"
#include "Registry.hpp"
#include <gtest/gtest.h>
#include <random>

TEST(
  KasaTests,
  RegistryPopulation_RandomizedProductCount_RegistrySizeMatchesGeneratedProductCount)
{
    Registry registry;
    std::random_device my_random_device;
    std::mt19937 my_generator(my_random_device());
    std::uniform_int_distribution<> distr(1, 100);
    int random_product_count = distr(my_generator);

    double price = 10.00;
    long identifier_aplifier = 10;
    for (int i = 0; i < random_product_count; ++i)
    {
        registry.add(Product(
          static_cast<long>(i * identifier_aplifier), "Product", price));
    }
    EXPECT_EQ(registry.getEntryCount(), random_product_count);
}

TEST(KasaTests,
     RegistryPopulation_IgnoreProductOfDuplicateId_ProductNotAddedToRegistry)
{
    Registry registry;
    long duplicate_identifier = 1;
    double random_price = 67.00;
    struct Product product_a(
      duplicate_identifier, "MyUniqueProduct", random_price);
    struct Product product_b(
      duplicate_identifier, "AnotherTotallyDifferentProduct", random_price);
    registry.add(product_a);
    registry.add(product_b);
    EXPECT_EQ(registry.getEntryCount(), 1);
}

TEST(
  KasaTests,
  RegistryDepopulation_RegistrySizeAfterDeletion_RegistrySizeIsDecrementedAfterDeletingOneProduct)
{
    Registry registry;
    std::random_device my_random_device;
    std::mt19937 my_generator(my_random_device());
    std::uniform_int_distribution<> distr(1, 100);
    int random_product_count = distr(my_generator);

    for (int i = 0; i < random_product_count; ++i)
    {
        registry.add(Product(i + 10, "Product", 10.00));
    }
    registry.del(registry.contents.begin()->second.identifier);
    EXPECT_EQ(registry.getEntryCount(), random_product_count - 1);
}
