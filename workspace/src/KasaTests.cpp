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

    double price = 10.00;
    long identifier_aplifier = 10;
    for (int i = 0; i < random_product_count; ++i)
    {
        struct Product temp_product;
        temp_product.identifier = static_cast<long>(i * identifier_aplifier);
        temp_product.name = "Product";
        temp_product.price = price;
        registry.add(temp_product);
    }
    EXPECT_EQ(registry.getEntryCount(), random_product_count);
}

TEST(KasaTests,
     ProductRegistration_IgnoreProductOfDuplicateId_ProductNotAddedToRegistry)
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
        temp_product.identifier = i + 10;
        temp_product.name = "Product";
        temp_product.price = 10.00;
        registry.add(temp_product);
    }
    registry.del(registry.contents.begin()->second.identifier);
    EXPECT_EQ(registry.getEntryCount(), random_product_count - 1);
}

TEST(
  KasaTests,
  ProductCartAddition_AddingRegisteredProduct_EmptyCartGainsSizeAfterAddingRegisteredProduct)
{
    Registry registry;
    Cart cart;

    registry.add({ 1, "apple", 5.00 });
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

    registry.add({ 1, "apple", 5.00 });
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

    registry.add({ 1, "apple", 5.00 });
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

    registry.add({ 1, "apple", 5.00 });
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

    registry.add({ 1, "apple", 5.00 });
    cart.del(1);
    EXPECT_EQ(cart.getEntryCount(), 0);
}

TEST(
  KasaTests,
  ProductCartDeletion_DeletingUnregisteredProduct_PopulatedCartSizeStaysTheSameWhenDeletingUnregisteredProduct)
{
    Registry registry;
    Cart cart;

    registry.add({ 1, "apple", 5.00 });
    cart.add(registry, 1);
    cart.del(2);
    EXPECT_EQ(cart.getEntryCount(), 1);
}

TEST(
  KasaTests,
  ProductCartTotalCalcuation_ExampleCartTotalCalculation_ProperlyCalculateExampleCart)
{
    Registry registry;
    registry.add({ 1, "apple", 5.00 });
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
    registry.add({ 1, "apple", 5.00 });
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
    registry.add({ 1, "apple", 5.00 });
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
    registry.add({ 1, "apple", 5.00 });
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
    registry.add({ 10, "apple", 10.00 });
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
    registry.add({ 10, "apple", 10.00 });
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
    registry.add({ 10, "apple", 10.00 });
    registry.add_promotion(10, 5);
    registry.activate_promotion(10);
    EXPECT_EQ(registry.promotions.at(10).is_active, true);
}

TEST(
  KasaTests,
  ProductPromotions_PromotionActivation_RegistryCorrectlyDeactivatesPromotion)
{
    Registry registry;
    registry.add({ 10, "apple", 10.00 });
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
    registry.add({ 10, "apple", 10.00 });
    registry.add({ 9, "banana", 10.00 });
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
    registry.add({ 10, "apple", 10.00 });
    registry.add({ 9, "banana", 10.00 });
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
    registry.add({ 10, "apple", 10.00 });
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
    registry.add({ 10, "apple", 10.00 });
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

TEST(KasaTests, demo)
{
    Registry registry;
    registry.add({ 1, "apple", 5.300 });
    registry.add({ 2, "banana", 15.00 });
    registry.add({ 3, "kiwi", 3.00 });
    registry.add_promotion(1, 0.2);
    registry.add_promotion(3, 3);
    Cart cart;
    cart.add(registry, 1);
    cart.add(registry, 3);
    std::cout << "Registered Items:" << std::endl;
    registry.print();
    std::cout << "Registered Promotions:" << std::endl;
    registry.print_promotions();
    std::cout << "Selected Items:" << std::endl;
    cart.print(registry);
    std::cout << "Current Total: " << cart.calculateValue(registry)
              << std::endl;
    int choice;
    while (true)
    {
        std::cout << "\n\n\nAvailable Commands:\n";
        std::cout << "1. Register Product\n";
        std::cout << "2. Deregister Product\n";
        std::cout << "3. Add to Cart\n";
        std::cout << "4. Remove from Cart\n";
        std::cout << "5. Calculate Total Value\n";
        std::cout << "6. Close Cart\n";
        std::cout << "7. Print Cart\n";
        std::cout << "8. Print Registry\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter choice: ";

        if (!(std::cin >> choice))
        {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice)
        {
            case 1:
            {
                long identifier;
                std::string name;
                double price;
                struct Product product;
                std::cout << "Enter Product ID (long): ";
                std::cin >> identifier;
                std::cout << "Enter Name: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                std::cout << "Enter Price: ";
                std::cin >> price;
                registry.add({ identifier, name, price });
                std::cout << "Product registered successfully.\n";
                break;
            }

            case 2:
            {
                long identifier;
                std::cout << "Enter Product ID to deregister: ";
                std::cin >> identifier;
                registry.del(identifier);
                break;
            }

            case 3:
            {
                long identifier;
                std::cout << "Enter Product ID to add to cart: ";
                std::cin >> identifier;
                cart.add(registry, identifier);
                break;
            }

            case 4:
            {
                long identifier;
                std::cout << "Enter Product ID to remove from cart: ";
                std::cin >> identifier;
                cart.del(identifier);
                break;
            }

            case 5:
            {
                std::cout << "Current Cart Value: "
                          << cart.calculateValue(registry) << "\n";
                break;
            }

            case 6:
            {
                cart.close(registry);
                std::cout << "Cart has been closed/cleared.\n";
                break;
            }

            case 7:
            {
                std::cout << "Currently selected products:" << std::endl;
                cart.print(registry);
                break;
            }

            case 8:
            {
                std::cout << "Currently registered products:" << std::endl;
                registry.print();
                break;
            }

            case 0:
                std::cout << "Exiting system...\n";
                return;

            default:
                std::cout << "Invalid option. Try again.\n";
                break;
        }
    }
}
