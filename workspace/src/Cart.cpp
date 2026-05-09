#include "Cart.hpp"
#include "Promotion.hpp"
#include <algorithm>
#include <numeric>

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

void Cart::print_promotions(Registry const& registry) const
{
    for (auto const& [identifier, promotion] : registry.promotions)
    {
        if (promotion.type == PromotionType::DISCOUNT)
        {
            auto iterator =
              std::find(contents.begin(), contents.end(), identifier);
            std::cout << "[ " << (promotion.is_active ? "unlocked" : "locked")
                      << " | "
                      << (iterator != contents.end() ? "applicable"
                                                     : "inapplicable")
                      << " | " << promotion.identifier << " -> "
                      << promotion.discount * 100 << "% OFF! ]" << std::endl;
        }
        else
        {
            int count =
              std::count(contents.begin(), contents.end(), identifier);
            std::cout << "[ " << (promotion.is_active ? "unlocked" : "locked")
                      << " | "
                      << (count >= promotion.nth_free ? "applicable"
                                                      : "inapplicable")
                      << " | " << promotion.identifier << " -> "
                      << promotion.nth_free - 1 << " +1 GRATIS!! ]"
                      << std::endl;
        }
    }
}
