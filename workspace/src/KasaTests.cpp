#include "Kasa.hpp"
#include <gtest/gtest.h>
#include <random>

TEST(
  KasaTests,
  ProductRegistration_RandomizedProductCount_RegistrySizeMatchesGeneratedProductCount)
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
        registerProduct(registry, temp_product);
    }
    EXPECT_EQ(registry.size(), random_product_count);
}

TEST(KasaTests,
     ProductRegistration_IgnoreProductOfDuplicateId_ProductNotAddedToRegistry)
{
    Registry registry;
    struct Product productA(1, "MyUniqueProduct", 10.00);
    struct Product productB(1, "AnotherTotallyDifferentProduct", 67.00);
    registerProduct(registry, productA);
    registerProduct(registry, productB);
    EXPECT_EQ(registry.size(), 1);
}

TEST(
  KasaTests,
  ProductInitialization_DefaultProductConstructorMembers_ProductCreatedWithNonParameterConstructorGivesDefaultMembers)
{
    struct Product default_product;
    // Although there are multiple things checked here, breaking AAA rule
    //  I decided they are logically connected to each other, so breaking
    //  them up into multiple tests would not be ideal
    EXPECT_EQ(default_product.id, 0);
    EXPECT_EQ(default_product.name, "");
    EXPECT_EQ(default_product.price, 0.0);
}

TEST(
  KasaTests,
  ProductInitialization_ProductParameteredConstructorMembers_ProductConstructorIdParameterMatchesObjectMember)
{
    double expected_id = 10;
    struct Product product(expected_id, "dummy", 1.00);
    EXPECT_EQ(product.id, expected_id);
}
TEST(
  KasaTests,
  ProductInitialization_ProductParameteredConstructorMembers_ProductConstructorNameParameterMatchesObjectMember)
{
    std::string expected_name = "product";
    struct Product product(1, expected_name, 1.00);
    EXPECT_EQ(product.name, expected_name);
}

TEST(
  KasaTests,
  ProductInitialization_ProductParameteredConstructorMembers_ProductConstructorPriceParameterMatchesObjectMember)
{
    double expected_price = 10.00;
    struct Product product(1, "dummy", expected_price);
    EXPECT_EQ(product.price, expected_price);
}

TEST(
  KasaTests,
  ProductDeregistration_RegistrySizeAfterDeletion_RegistrySizeIsDecrementedAfterDeletingOneProduct)
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
        registerProduct(registry, temp_product);
    }
    deregisterProduct(registry, registry.at(0).id);
    EXPECT_EQ(registry.size(), random_product_count - 1);
}
