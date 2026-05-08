#include "Kasa.hpp"
#include "Product.hpp"
#include <gtest/gtest.h>
#include <random>

TEST(
  KasaTests,
  ProductPromotions_ActivatingLoyaltyCard_RegistryActivatesLoyaltyCardViaMethod)
{
    Registry registry;
    registry.activate_loyalty_card();
    EXPECT_EQ(registry.loyalty_card_active, true);
}

TEST(
  KasaTests,
  ProductPromotions_DeactivatingLoyaltyCard_RegistryDeactivatesLoyaltyCardViaMethod)
{
    Registry registry;
    registry.activate_loyalty_card();
    registry.deactivate_loyalty_card();
    EXPECT_EQ(registry.loyalty_card_active, false);
}

TEST(
  KasaTests,
  ProductPromotions_PromotionRegistration_RegistryCorrectlyRegistersDiscountPromotion)
{
    Registry registry;
    registry.add(Product(10, "apple", 10.00));
    registry.add_promotion(10, 0.2);
    EXPECT_EQ(registry.promotions.at(10).type, PromotionType::DISCOUNT);
    EXPECT_EQ(registry.promotions.at(10).discount, 0.2);
    EXPECT_EQ(registry.promotions.at(10).nth_free, 0);
    EXPECT_EQ(registry.promotions.at(10).is_active, false);
}

TEST(
  KasaTests,
  ProductPromotions_PromotionRegistration_RegistryDoesNotRegisterDiscountPromotionForInvalidProduct)
{
    Registry registry;
    registry.add_promotion(10, 0.2);
    EXPECT_EQ(registry.promotions.size(), 0);
}

TEST(
  KasaTests,
  ProductPromotions_PromotionRegistration_RegistryCorrectlyRegistersBulkPromotion)
{
    Registry registry;
    registry.add(Product(10, "apple", 10.00));
    registry.add_promotion(10, 5);
    EXPECT_EQ(registry.promotions.at(10).type, PromotionType::BULK);
    EXPECT_EQ(registry.promotions.at(10).discount, 0.0);
    EXPECT_EQ(registry.promotions.at(10).nth_free, 5);
    EXPECT_EQ(registry.promotions.at(10).is_active, false);
}

TEST(
  KasaTests,
  ProductPromotions_PromotionRegistration_RegistryDoesNotRegisterBulkPromotionForInvalidProduct)
{
    Registry registry;
    registry.add_promotion(10, 3);
    EXPECT_EQ(registry.promotions.size(), 0);
}

TEST(KasaTests,
     ProductPromotions_PromotionActivation_RegistryCorrectlyActivatesPromotion)
{
    Registry registry;
    registry.add(Product(10, "apple", 10.00));
    registry.add_promotion(10, 5);
    registry.activate_promotion(10);
    EXPECT_EQ(registry.promotions.at(10).is_active, true);
}

TEST(
  KasaTests,
  ProductPromotions_PromotionActivation_RegistryCorrectlyDeactivatesPromotion)
{
    Registry registry;
    registry.add(Product(10, "apple", 10.00));
    registry.add_promotion(10, 5);
    registry.activate_promotion(10);
    registry.deactivate_promotion(10);
    EXPECT_EQ(registry.promotions.at(10).is_active, false);
}

TEST(
  KasaTests,
  ProductPromotions_PromotionStatusUpdateViaLoyaltyCard_RegistryCorrectlyActivatesPromotionsBasedOnActivatedLoyaltyCard)
{
    Registry registry;
    registry.add(Product(10, "apple", 10.00));
    registry.add(Product(9, "banana", 10.00));
    registry.add_promotion(10, 5);
    registry.add_promotion(9, 0.2);
    registry.activate_loyalty_card();
    registry.update_promotion_status();
    EXPECT_EQ(registry.promotions.at(10).is_active, true);
    EXPECT_EQ(registry.promotions.at(9).is_active, true);
}

TEST(
  KasaTests,
  ProductPromotions_PromotionStatusUpdateViaLoyaltyCard_RegistryCorrectlyActivatesPromotionsBasedOnDeactivatedLoyaltyCard)
{
    Registry registry;
    registry.add(Product(10, "apple", 10.00));
    registry.add(Product(9, "banana", 10.00));
    registry.add_promotion(10, 5);
    registry.add_promotion(9, 0.2);

    registry.activate_loyalty_card();
    registry.update_promotion_status();

    registry.deactivate_loyalty_card();
    registry.update_promotion_status();

    EXPECT_EQ(registry.promotions.at(10).is_active, false);
    EXPECT_EQ(registry.promotions.at(9).is_active, false);
}

TEST(
  KasaTests,
  ProductCardAddition_RegisteringCardViaProductAdd_LoyaltyCardActivatesWhenRegisteringCorrectIdentifier)
{
    Registry registry;
    Cart cart;
    registry.add(Product(10, "apple", 10.00));
    registry.add_promotion(10, 5);
    cart.add(registry, loyalty_card_identifier);
    EXPECT_EQ(registry.promotions.at(10).is_active, true);
}

TEST(
  KasaTests,
  ProductCardAddition_UnregisteringCardViaProductAdd_LoyaltyCardDeactivatesWhenRegisteringCorrectIdentifier)
{
    Registry registry;
    Cart cart;
    registry.add(Product(10, "apple", 10.00));
    registry.add_promotion(10, 5);
    cart.add(registry, loyalty_card_identifier);
    cart.add(registry, loyalty_card_identifier);
    EXPECT_EQ(registry.promotions.at(10).is_active, false);
}

TEST(
  KasaTests,
  ProductCartClosing_LoyaltyCardDeactivation_CardIsDeactivatedWhenClosingCart)
{
    Registry registry;
    Cart cart;
    cart.add(registry, loyalty_card_identifier);
    cart.close(registry);
    EXPECT_EQ(registry.loyalty_card_active, false);
}
