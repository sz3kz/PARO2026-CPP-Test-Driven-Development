#include "Registry.hpp"
#include "World.hpp"

void Registry::add(struct Product const& product)
{
    if (product.identifier != invalid_identifier)
    {
        contents.emplace(product.identifier, product);
    }
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

void Registry::add_promotion(long identifier, double discount)
{
    // Registers a DISCOUNT-type promotion.
    // Only if identifier is of a registered product.
    auto iterator = contents.find(identifier);
    if (iterator == contents.end())
    {
        return;
    }
    promotions.emplace(
      identifier,
      Promotion(identifier, PromotionType::DISCOUNT, discount, 0, false));
}

/**
 * @brief Registers a new bulk-type promotion for a specific product.
 * * This function creates and stores a `Promotion` object of type `BULK`
 * associated with the provided product identifier. The promotion is only
 * added if the product ID already exists within the registry's contents.
 * #AIGenerated
 * * @param identifier The unique long identifier of the product to receive the
 * promotion.
 * @param nth_free An integer specifying the "buy N, get one free" threshold
 * (e.g., if nth_free is 3, every 3rd item is free).
 * * @note If the @p identifier is not found in the @c contents map, the
 * function returns silently without adding a promotion.
 * * @see Promotion, PromotionType
 */
void Registry::add_promotion(long identifier, int nth_free)
{
    auto iterator = contents.find(identifier);
    if (iterator == contents.end())
    {
        return;
    }
    promotions.emplace(
      identifier,
      Promotion(identifier, PromotionType::BULK, 0.0, nth_free, false));
}

void Registry::activate_promotion(long identifier)
{
    auto iterator = promotions.find(identifier);
    if (iterator == promotions.end())
    {
        return;
    }
    Promotion promotion = iterator->second;
    promotion.is_active = true;
    promotions[identifier] = promotion;
}

void Registry::deactivate_promotion(long identifier)
{
    auto iterator = promotions.find(identifier);
    if (iterator == promotions.end())
    {
        return;
    }
    Promotion promotion = iterator->second;
    promotion.is_active = false;
    promotions[identifier] = promotion;
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
