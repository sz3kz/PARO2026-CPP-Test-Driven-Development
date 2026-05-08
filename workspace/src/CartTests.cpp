#include "Kasa.hpp"
#include "Product.hpp"
#include <gtest/gtest.h>
#include <random>

TEST(
  KasaTests,
  ProductCartAddition_AddingRegisteredProduct_EmptyCartGainsSizeAfterAddingRegisteredProduct)
{
    Registry registry;
    Cart cart;

    registry.add(Product(1, "apple", 5.00));
    cart.add(registry, 1);
    EXPECT_EQ(cart.getEntryCount(), 1);
}

TEST(
  KasaTests,
  ProductCartAddition_AddingUnregisteredProduct_EmptyCartRemainsEmptyAfterAddingUnregisteredProduct)
{
    Registry registry;
    Cart cart;
    cart.add(registry, 1);
    EXPECT_EQ(cart.getEntryCount(), 0);
}

TEST(KasaTests, ProductCartAddition_AddProductOfDuplicateId_ProductAddedToCart)
{
    Registry registry;
    Cart cart;

    registry.add(Product(1, "apple", 5.00));
    cart.add(registry, 1);
    cart.add(registry, 1);
    EXPECT_EQ(cart.getEntryCount(), 2);
}

TEST(
  KasaTests,
  ProductCartAddition_RandomizedProductCount_CartSizeMatchesGeneratedProductCount)
{
    Registry registry;
    Cart cart;
    std::random_device my_random_device;
    std::mt19937 my_generator(my_random_device());
    std::uniform_int_distribution<> distr(1, 100);
    int random_product_count = distr(my_generator);

    registry.add(Product(1, "apple", 5.00));
    for (int i = 0; i < random_product_count; ++i)
    {
        cart.add(registry, 1);
    }
    EXPECT_EQ(cart.getEntryCount(), random_product_count);
}

TEST(
  KasaTests,
  ProductCartDeletion_DeletingRegisteredProduct_PopulatedCartDeincrementsSizeWhenDeletingRegisteredProduct)
{
    Registry registry;
    Cart cart;

    registry.add(Product(1, "apple", 5.00));
    cart.add(registry, 1);
    cart.del(1);
    EXPECT_EQ(cart.getEntryCount(), 0);
}

TEST(
  KasaTests,
  ProductCartDeletion_DeletingRegisteredProductFromEmptyCart_EmptyCartDoesNotChangeSizeWhenDeletingRegisteredProduct)
{
    Registry registry;
    Cart cart;

    registry.add(Product(1, "apple", 5.00));
    cart.del(1);
    EXPECT_EQ(cart.getEntryCount(), 0);
}

TEST(
  KasaTests,
  ProductCartDeletion_DeletingUnregisteredProduct_PopulatedCartSizeStaysTheSameWhenDeletingUnregisteredProduct)
{
    Registry registry;
    Cart cart;

    registry.add(Product(1, "apple", 5.00));
    cart.add(registry, 1);
    cart.del(2);
    EXPECT_EQ(cart.getEntryCount(), 1);
}

TEST(
  KasaTests,
  ProductCartTotalCalcuation_ExampleCartTotalCalculation_ProperlyCalculateExampleCart)
{
    Registry registry;
    registry.add(Product(1, "apple", 5.00));
    Cart cart;
    std::random_device my_random_device;
    std::mt19937 my_generator(my_random_device());
    std::uniform_int_distribution<> distr(1, 100);
    int random_product_count = distr(my_generator);

    double price = 5.00;
    for (int _ = 0; _ < random_product_count; ++_)
    {
        cart.add(registry, 1);
    }

    EXPECT_EQ(cart.calculateValue(registry), price * random_product_count);
}

TEST(
  KasaTests,
  ProductCartTotalCalcuation_ExampleCartTotalCalculationWithDiscount_ProperlyCalculateExampleCartWithDiscount)
{
    Registry registry;
    registry.add(Product(1, "apple", 5.00));
    registry.add_promotion(1, 0.2);
    registry.activate_loyalty_card();
    registry.update_promotion_status();
    Cart cart;
    std::random_device my_random_device;
    std::mt19937 my_generator(my_random_device());
    std::uniform_int_distribution<> distr(1, 100);
    int random_product_count = distr(my_generator);

    double price = 5.00;
    for (int _ = 0; _ < random_product_count; ++_)
    {
        cart.add(registry, 1);
    }

    EXPECT_EQ(cart.calculateValue(registry),
              price * random_product_count * (1 - 0.2));
}

TEST(
  KasaTests,
  ProductCartTotalCalcuation_ExampleCartTotalCalculationWithBulk_ProperlyCalculateExampleCartWithBulk)
{
    Registry registry;
    registry.add(Product(1, "apple", 5.00));
    registry.add_promotion(1, 3);
    registry.activate_loyalty_card();
    registry.update_promotion_status();
    Cart cart;
    std::random_device my_random_device;
    std::mt19937 my_generator(my_random_device());
    std::uniform_int_distribution<> distr(1, 100);
    int random_product_count = distr(my_generator);

    double price = 5.00;
    for (int _ = 0; _ < random_product_count; ++_)
    {
        cart.add(registry, 1);
    }

    EXPECT_EQ(cart.calculateValue(registry),
              (random_product_count -
               random_product_count / registry.promotions.at(1).nth_free) *
                price);
}

TEST(
  KasaTests,
  ProductCartClosing_RandomizedCartLengthClosing_CartIsFullyEmptiedByDeletingEveryProductIdentifier)
{
    Registry registry;
    registry.add(Product(1, "apple", 5.00));
    Cart cart;
    std::random_device my_random_device;
    std::mt19937 my_generator(my_random_device());
    std::uniform_int_distribution<> distr(1, 100);
    int random_product_count = distr(my_generator);

    for (int _ = 0; _ < random_product_count; ++_)
    {
        cart.add(registry, 1);
    }
    cart.close(registry);

    EXPECT_EQ(cart.getEntryCount(), 0);
}
