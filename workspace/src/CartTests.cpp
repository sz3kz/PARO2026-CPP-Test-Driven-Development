#include "Cart.hpp"
#include "Product.hpp"
#include "Registry.hpp"
#include <gtest/gtest.h>
#include <random>

TEST(
  KasaTests,
  CartAddition_AddingRegisteredProduct_EmptyCartGainsSizeAfterAddingRegisteredProduct)
{
    Registry registry;
    Cart cart;

    registry.add(Product(1, "apple", 5.00));
    cart.add(registry, 1);
    EXPECT_EQ(cart.getEntryCount(), 1);
}

TEST(
  KasaTests,
  CartAddition_AddingUnregisteredProduct_EmptyCartRemainsEmptyAfterAddingUnregisteredProduct)
{
    Registry registry;
    Cart cart;
    cart.add(registry, 1);
    EXPECT_EQ(cart.getEntryCount(), 0);
}

TEST(KasaTests, CartAddition_AddProductOfDuplicateId_ProductAddedToCart)
{
    Registry registry;
    Cart cart;

    registry.add(Product(1, "apple", 5.00));
    cart.add(registry, 1);
    cart.add(registry, 1);
    EXPECT_EQ(cart.getEntryCount(), 2);
}

TEST(KasaTests,
     CartAddition_RandomizedProductCount_CartSizeMatchesGeneratedProductCount)
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
  CartDeletion_DeletingRegisteredProduct_PopulatedCartDeincrementsSizeWhenDeletingRegisteredProduct)
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
  CartDeletion_DeletingRegisteredProductFromEmptyCart_EmptyCartDoesNotChangeSizeWhenDeletingRegisteredProduct)
{
    Registry registry;
    Cart cart;

    registry.add(Product(1, "apple", 5.00));
    cart.del(1);
    EXPECT_EQ(cart.getEntryCount(), 0);
}

TEST(
  KasaTests,
  CartDeletion_DeletingUnregisteredProduct_PopulatedCartSizeStaysTheSameWhenDeletingUnregisteredProduct)
{
    Registry registry;
    Cart cart;

    registry.add(Product(1, "apple", 5.00));
    cart.add(registry, 1);
    cart.del(2);
    EXPECT_EQ(cart.getEntryCount(), 1);
}

TEST(KasaTests,
     CartTotalCalculation_EmptyCartValue_EmptyCartShouldBeWorthNothing)
{
    Registry registry;
    Cart cart;
    EXPECT_EQ(cart.calculateValue(registry), 0.0);
}

TEST(KasaTests,
     CartTotalCalculation_VariableCartValue_CartShouldDisplayCorrectValue)
{
    Registry registry;
    Cart cart;

    registry.add(Product(1, "apple", 5.00));
    registry.add(Product(2, "banana", 6.00));
    registry.add(Product(3, "kiwi", 3.00));
    cart.add(registry, 1);
    cart.add(registry, 2);
    cart.add(registry, 3);
    EXPECT_EQ(cart.calculateValue(registry), 5.00 + 6.00 + 3.00);
}

TEST(
  KasaTests,
  CartTotalCalculation_CartValueUpdate_CartValueShouldUpdateCorrectlyAfterChange)
{
    Registry registry;
    Cart cart;
    registry.add(Product(1, "apple", 5.00));
    registry.add(Product(2, "banana", 6.00));
    cart.add(registry, 1);
    cart.add(registry, 2);
    double value0 = cart.calculateValue(registry);
    cart.del(1);
    double value1 = cart.calculateValue(registry);
    EXPECT_EQ(value1 - value0, -5.00);
}

TEST(
  KasaTests,
  CartTotalCalculation_ExampleCartTotalCalculation_ProperlyCalculateExampleCart)
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
  CartTotalCalculation_CartTotalCalculationWithFreeItem_ProperlyCalculateCartToZero)
{
    Registry registry;
    registry.add(Product(1, "apple", 0.00));
    Cart cart;
    cart.add(registry, 1);
    EXPECT_EQ(cart.calculateValue(registry), 0.00);
}

TEST(
  KasaTests,
  CartTotalCalculation_ExampleCartTotalCalculationWithDiscount_ProperlyCalculateExampleCartWithDiscount)
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
  CartTotalCalculation_ExampleCartTotalCalculationWithBulk_ProperlyCalculateExampleCartWithBulk)
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
  CartTotalCalculation_ExampleCartTotalCalculationWithPromotions_ProperlyCaculateExampleCartWithPromotions)
{
    Registry registry;
    registry.add(Product(1, "apple", 10.00));
    registry.add_promotion(1, 5);
    registry.add(Product(2, "kiwi", 10.00));
    registry.add_promotion(2, 0.2);
    registry.add(Product(3, "banana", 10.00));
    Cart cart;
    cart.add(registry, 1);
    cart.add(registry, 1);
    cart.add(registry, 1);
    cart.add(registry, 1);
    cart.add(registry, 1);
    cart.add(registry, 2);
    cart.add(registry, 3);
    cart.add(registry, loyalty_card_identifier);
    registry.update_promotion_status();
    EXPECT_EQ(cart.calculateValue(registry),
              10.00 * 4 + 10.00 * (1 - 0.2) + 10.00);
}

TEST(
  KasaTests,
  CartTotalCalculation_CartValueOfBulkPromotionWhenDeletingItem_CartRevertsToNormalPricingIfBulkPromotionDoesNotHoldAfterDelete)
{
    Registry registry;
    Cart cart;
    registry.add(Product(1, "apple", 10.00));
    registry.add_promotion(1, 5);
    cart.add(registry, 1);
    cart.add(registry, 1);
    cart.add(registry, 1);
    cart.add(registry, 1);
    cart.add(registry, 1);
    cart.add(registry, loyalty_card_identifier);
    registry.update_promotion_status();
    cart.del(1);
    EXPECT_EQ(cart.calculateValue(registry), 10.00 * 4);
}

TEST(
  KasaTests,
  CartTotalCalculation_CartValueOfBulkPromotionWhenProductFree_CartValueIsZero)
{
    Registry registry;
    Cart cart;
    registry.add(Product(1, "apple", 0.00));
    registry.add_promotion(1, 5);
    cart.add(registry, 1);
    cart.add(registry, 1);
    cart.add(registry, 1);
    cart.add(registry, 1);
    cart.add(registry, 1);
    cart.add(registry, loyalty_card_identifier);
    registry.update_promotion_status();
    EXPECT_EQ(cart.calculateValue(registry), 0.00);
}

TEST(
  KasaTests,
  CartTotalCalculation_CartValueOfDiscountPromotionWhenProductFree_CartValueIsZero)
{
    Registry registry;
    Cart cart;
    registry.add(Product(1, "apple", 0.00));
    registry.add_promotion(1, 0.5);
    cart.add(registry, 1);
    cart.add(registry, loyalty_card_identifier);
    registry.update_promotion_status();
    EXPECT_EQ(cart.calculateValue(registry), 0.00);
}

TEST(
  KasaTests,
  CartTotalCalculation_ExampleCartTotalCalculationWithoutPromotions_ProperlyCaculateExampleCartWithoutPromotions)
{
    Registry registry;
    registry.add(Product(1, "apple", 10.00));
    registry.add_promotion(1, 5);
    registry.add(Product(2, "kiwi", 10.00));
    registry.add_promotion(2, 0.2);
    registry.add(Product(3, "banana", 10.00));
    Cart cart;
    cart.add(registry, 1);
    cart.add(registry, 1);
    cart.add(registry, 1);
    cart.add(registry, 1);
    cart.add(registry, 1);
    cart.add(registry, 2);
    cart.add(registry, 3);
    cart.add(registry, loyalty_card_identifier);
    registry.update_promotion_status();
    cart.add(registry, loyalty_card_identifier);
    registry.update_promotion_status();
    EXPECT_EQ(cart.calculateValue(registry), 10.00 * 5 + 10.00 + 10.00);
}

TEST(
  KasaTests,
  CartTotalCalculation_PromotionCalculationAfterCardActivation_ProperlyAppliesPromotionsAfterLoyaltyCardWasScanned)
{
    Registry registry;
    registry.add(Product(1, "apple", 10.00));
    registry.add_promotion(1, 5);
    registry.add(Product(2, "kiwi", 10.00));
    registry.add_promotion(2, 0.2);
    registry.add(Product(3, "banana", 10.00));
    Cart cart;
    cart.add(registry, 1);
    cart.add(registry, 1);
    cart.add(registry, loyalty_card_identifier);
    registry.update_promotion_status();
    cart.add(registry, 1);
    cart.add(registry, 1);
    cart.add(registry, 1);
    cart.add(registry, 2);
    cart.add(registry, 3);
    EXPECT_EQ(cart.calculateValue(registry),
              10.00 * 4 + 10.00 * (1 - 0.2) + 10.00);
}

TEST(
  KasaTests,
  CartClosing_RandomizedCartLengthClosing_CartIsFullyEmptiedByDeletingEveryProductIdentifier)
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

TEST(KasaTests,
     CartClosing_RandomizedCartValueClosing_CartValueIsZeroAfterClosing)
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

    EXPECT_EQ(cart.calculateValue(registry), 0.00);
}
