#include "Kasa.hpp"
#include "Product.hpp"
#include <gtest/gtest.h>
#include <random>

TEST(KasaTests, demo)
{
    Registry registry;
    registry.add(Product(1, "apple", 5.300));
    registry.add(Product(2, "banana", 15.00));
    registry.add(Product(3, "kiwi", 3.00));
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
                std::cout << "Enter Product ID (long): ";
                std::cin >> identifier;
                std::cout << "Enter Name: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                std::cout << "Enter Price: ";
                std::cin >> price;
                registry.add(Product(identifier, name, price));
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
