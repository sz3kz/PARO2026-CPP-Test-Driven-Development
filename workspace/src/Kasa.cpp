#include "Kasa.hpp"
#include "Cart.hpp"
#include "Product.hpp"
#include "Promotion.hpp"
#include "Registry.hpp"
#include <algorithm>
#include <numeric>

void Registry::add(struct Product const& product)
{
    contents.emplace(product.identifier, product);
}

void Registry::activate_loyalty_card()
{
    loyalty_card_active = true;
}

void Registry::deactivate_loyalty_card()
{
    loyalty_card_active = false;
}

void Registry::update_promotion_status()
{
    for (auto const& pair : promotions)
    {
        if (loyalty_card_active)
        {
            activate_promotion(pair.first);
        }
        else
        {
            deactivate_promotion(pair.first);
        }
    }
}

void Registry::add_promotion(long identity, double discount)
{
    // Registers a DISCOUNT-type promotion.
    // Only if identifier is of a registered product.
    auto iterator = contents.find(identity);
    if (iterator == contents.end())
    {
        return;
    }
    promotions.emplace(
      identity,
      Promotion(identity, PromotionType::DISCOUNT, discount, 0, false));
}

/**
 * @brief Registers a new bulk-type promotion for a specific product.
 * * This function creates and stores a `Promotion` object of type `BULK`
 * associated with the provided product identifier. The promotion is only
 * added if the product ID already exists within the registry's contents.
 * #AIGenerated
 * * @param identity The unique long identifier of the product to receive the
 * promotion.
 * @param nth_free An integer specifying the "buy N, get one free" threshold
 * (e.g., if nth_free is 3, every 3rd item is free).
 * * @note If the @p identity is not found in the @c contents map, the function
 * returns silently without adding a promotion.
 * * @see Promotion, PromotionType
 */
void Registry::add_promotion(long identity, int nth_free)
{
    auto iterator = contents.find(identity);
    if (iterator == contents.end())
    {
        return;
    }
    promotions.emplace(
      identity, Promotion(identity, PromotionType::BULK, 0.0, nth_free, false));
}

void Registry::activate_promotion(long identity)
{
    auto iterator = promotions.find(identity);
    if (iterator == promotions.end())
    {
        return;
    }
    Promotion promotion = iterator->second;
    promotion.is_active = true;
    promotions[identity] = promotion;
}

void Registry::deactivate_promotion(long identity)
{
    auto iterator = promotions.find(identity);
    if (iterator == promotions.end())
    {
        return;
    }
    Promotion promotion = iterator->second;
    promotion.is_active = false;
    promotions[identity] = promotion;
}

void Registry::del(long identifier)
{
    contents.erase(identifier);
}

[[nodiscard]]
auto Registry::getEntryCount() const -> size_t
{
    return contents.size();
}

void Registry::print() const
{
    for (auto const& pair : contents)
    {
        std::cout << pair.second << '\n';
    }
}

void Registry::print_promotions() const
{
    for (auto const& pair : promotions)
    {
        std::cout << pair.second << '\n';
    }
}

void Cart::add(Registry& registry, long identifier)
{
    if (identifier == loyalty_card_identifier)
    {
        if (registry.loyalty_card_active == false)
        {
            registry.activate_loyalty_card();
        }
        else
        {
            registry.deactivate_loyalty_card();
        }
        registry.update_promotion_status();
    }
    if (registry.contents.contains(identifier))
    {
        contents.push_back(identifier);
    }
}

void Cart::del(long identifier)
{
    auto iterator = std::find_if(contents.begin(),
                                 contents.end(),
                                 [identifier](auto current_identifier)
                                 { return current_identifier == identifier; });
    if (iterator == contents.end())
    {
        return;
    }
    contents.erase(iterator);
}

[[nodiscard]]
auto Cart::calculateValue(Registry const& registry) const -> double
{
    double total = std::accumulate(
      contents.begin(),
      contents.end(),
      0.0,
      [registry](auto accumulator, auto identifier)
      { return accumulator + registry.contents.at(identifier).price; });

    std::map<long, int> identifier2amount;
    for (auto const& identifier : contents)
    {
        identifier2amount[identifier]++;
    }

    // BULK calculation
    for (auto const& [identifier, amount] : identifier2amount)
    {
        bool identifier_not_associated_with_promotion =
          (registry.promotions.count(identifier) == 0);

        if (identifier_not_associated_with_promotion)
        {
            continue;
        }

        bool identifier_has_wrong_promotion_type =
          (registry.promotions.at(identifier).type != PromotionType::BULK);
        bool promotion_not_active =
          (registry.promotions.at(identifier).is_active == false);

        if (identifier_has_wrong_promotion_type || promotion_not_active)
        {
            continue;
        }
        total -= (amount / registry.promotions.at(identifier).nth_free) *
                 registry.contents.at(identifier).price;
    }

    // DISCOUNT calculation
    for (auto const& [identifier, amount] : identifier2amount)
    {
        bool identifier_not_associated_with_promotion =
          (registry.promotions.count(identifier) == 0);
        if (identifier_not_associated_with_promotion)
        {
            continue;
        }
        bool identifier_has_wrong_promotion_type =
          (registry.promotions.at(identifier).type != PromotionType::DISCOUNT);
        bool promotion_not_active =
          (registry.promotions.at(identifier).is_active == false);

        if (identifier_has_wrong_promotion_type || promotion_not_active)
        {
            continue;
        }
        total -= amount * registry.promotions.at(identifier).discount *
                 registry.contents.at(identifier).price;
    }
    return total;
}

[[nodiscard]]
auto Cart::getEntryCount() const -> size_t
{
    return contents.size();
}

void Cart::close(Registry& registry)
{
    for (auto identifier : contents)
    {
        del(identifier);
    }
    registry.deactivate_loyalty_card();
}

void Cart::print(Registry const& registry) const
{
    for (auto identifier : contents)
    {
        std::cout << registry.contents.at(identifier) << '\n';
    }
}

auto operator<<(std::ostream& output_stream,
                struct Product const& product) -> std::ostream&
{
    output_stream << "(" << product.identifier << "|\"" << product.name << "\"|"
                  << product.price << ")";
    return output_stream;
}

auto operator<<(std::ostream& output_stream,
                struct Promotion const& promotion) -> std::ostream&
{
    if (promotion.type == PromotionType::DISCOUNT)
    {
        output_stream << "[ " << (promotion.is_active ? "active" : "inactive")
                      << " | " << promotion.identity << " -> "
                      << promotion.discount * 100 << "% OFF! ]";
    }
    else
    {
        output_stream << "[ " << (promotion.is_active ? "active" : "inactive")
                      << " | " << promotion.identity << " -> "
                      << promotion.nth_free - 1 << " +1 GRATIS!! ]";
    }
    return output_stream;
}
