#include "Kasa.hpp"
#include <gtest/gtest.h>
#include <random>

TEST(
  KasaTests,
  ProductInitialization_DefaultProductConstructorMembers_ProductCreatedWithNonParameterConstructorGivesDefaultMembers)
{
    struct Product default_product;
    // Although there are multiple things checked here, breaking AAA rule
    //  I decided they are logically connected to each other, so breaking
    //  them up into multiple tests would not be ideal
    EXPECT_EQ(default_product.identifier, 0);
    EXPECT_EQ(default_product.name, "");
    EXPECT_EQ(default_product.price, 0.0);
}

TEST(
  KasaTests,
  ProductInitialization_ProductParameteredConstructorMembers_ProductConstructorIdParameterMatchesObjectMember)
{
    long expected_identifier = 10;
    struct Product product(expected_identifier, "dummy", 1.00);
    EXPECT_EQ(product.identifier, expected_identifier);
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
