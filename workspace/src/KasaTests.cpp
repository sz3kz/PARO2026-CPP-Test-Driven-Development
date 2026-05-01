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
        registerProduct(registry, temp_product);
    }
    EXPECT_EQ(registry.size(), random_product_count);
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
    registerProduct(registry, product_a);
    registerProduct(registry, product_b);
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
        registerProduct(registry, temp_product);
    }
    deregisterProduct(registry, registry.begin()->second.identifier);
    EXPECT_EQ(registry.size(), random_product_count - 1);
}

TEST(
  KasaTests,
  ProductCartAddition_AddingRegisteredProduct_EmptyCartGainsSizeAfterAddingRegisteredProduct)
{
    Registry registry;
    Cart cart;

    registerProduct(registry, { 1, "apple", 5.00 });
    cartAddProduct(registry, cart, 1);
    EXPECT_EQ(cart.size(), 1);
}

TEST(
  KasaTests,
  ProductCartAddition_AddingUnregisteredProduct_EmptyCartRemainsEmptyAfterAddingUnregisteredProduct)
{
    Registry registry;
    Cart cart;
    cartAddProduct(registry, cart, 1);
    EXPECT_EQ(cart.size(), 0);
}

TEST(KasaTests, ProductCartAddition_AddProductOfDuplicateId_ProductAddedToCart)
{
    Registry registry;
    Cart cart;

    registerProduct(registry, { 1, "apple", 5.00 });
    cartAddProduct(registry, cart, 1);
    cartAddProduct(registry, cart, 1);
    EXPECT_EQ(cart.size(), 2);
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

    registerProduct(registry, { 1, "apple", 5.00 });
    for (int i = 0; i < random_product_count; ++i)
    {
        cartAddProduct(registry, cart, 1);
    }
    EXPECT_EQ(cart.size(), random_product_count);
}

TEST(
  KasaTests,
  ProductCartDeletion_DeletingRegisteredProduct_PopulatedCartDeincrementsSizeWhenDeletingRegisteredProduct)
{
    Registry registry;
    Cart cart;

    registerProduct(registry, { 1, "apple", 5.00 });
    cartAddProduct(registry, cart, 1);
    cartDeleteProduct(cart, 1);
    EXPECT_EQ(cart.size(), 0);
}

TEST(
  KasaTests,
  ProductCartDeletion_DeletingRegisteredProductFromEmptyCart_EmptyCartDoesNotChangeSizeWhenDeletingRegisteredProduct)
{
    Registry registry;
    Cart cart;

    registerProduct(registry, { 1, "apple", 5.00 });
    cartDeleteProduct(cart, 1);
    EXPECT_EQ(cart.size(), 0);
}

TEST(
  KasaTests,
  ProductCartDeletion_DeletingUnregisteredProduct_PopulatedCartSizeStaysTheSameWhenDeletingUnregisteredProduct)
{
    Registry registry;
    Cart cart;

    registerProduct(registry, { 1, "apple", 5.00 });
    cartAddProduct(registry, cart, 1);
    cartDeleteProduct(cart, 2);
    EXPECT_EQ(cart.size(), 1);
}

TEST(
  KasaTests,
  ProductCartTotalCalcuation_ExampleCartTotalCalculation_ProperlyCalculateExampleCart)
{
    Registry registry;
    registerProduct(registry, { 1, "apple", 5.00 });
    Cart cart;
    std::random_device my_random_device;
    std::mt19937 my_generator(my_random_device());
    std::uniform_int_distribution<> distr(1, 100);
    int random_product_count = distr(my_generator);

    double price = 5.00;
    for (int _ = 0; _ < random_product_count; ++_)
    {
        cartAddProduct(registry, cart, 1);
    }

    EXPECT_EQ(calculateCartValue(registry, cart), price * random_product_count);
}

TEST(
  KasaTests,
  ProductCartClosing_RandomizedCartLengthClosing_CartIsFullyEmptiedByDeletingEveryProductIdentifier)
{
    Registry registry;
    registerProduct(registry, { 1, "apple", 5.00 });
    Cart cart;
    std::random_device my_random_device;
    std::mt19937 my_generator(my_random_device());
    std::uniform_int_distribution<> distr(1, 100);
    int random_product_count = distr(my_generator);

    for (int _ = 0; _ < random_product_count; ++_)
    {
        cartAddProduct(registry, cart, 1);
    }
    cartClose(cart);

    EXPECT_EQ(cart.size(), 0);
}
TEST(KasaTests, demo)
{
    Registry registry;
    registerProduct(registry, { 1, "apple", 5.300 });
    registerProduct(registry, { 2, "banana", 15.00 });
    registerProduct(registry, { 3, "kiwi", 3.00 });
    Cart cart;
    cartAddProduct(registry, cart, 1);
    cartAddProduct(registry, cart, 3);
    std::cout << "Registered Items:" << std::endl;
    printRegistryProducts(registry);
    std::cout << "Selected Items:" << std::endl;
    printCartProducts(registry, cart);
    std::cout << "Current Total: " << calculateCartValue(registry, cart)
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
                registerProduct(registry, { identifier, name, price });
                std::cout << "Product registered successfully.\n";
                break;
            }

            case 2:
            {
                long identifier;
                std::cout << "Enter Product ID to deregister: ";
                std::cin >> identifier;
                deregisterProduct(registry, identifier);
                break;
            }

            case 3:
            {
                long identifier;
                std::cout << "Enter Product ID to add to cart: ";
                std::cin >> identifier;
                cartAddProduct(registry, cart, identifier);
                break;
            }

            case 4:
            {
                long identifier;
                std::cout << "Enter Product ID to remove from cart: ";
                std::cin >> identifier;
                cartDeleteProduct(cart, identifier);
                break;
            }

            case 5:
            {
                std::cout << "Current Cart Value: "
                          << calculateCartValue(registry, cart) << "\n";
                break;
            }

            case 6:
            {
                cartClose(cart);
                std::cout << "Cart has been closed/cleared.\n";
                break;
            }

            case 7:
            {
                std::cout << "Currently selected products:" << std::endl;
                printCartProducts(registry, cart);
                break;
            }

            case 8:
            {
                std::cout << "Currently registered products:" << std::endl;
                printRegistryProducts(registry);
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
