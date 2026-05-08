#pragma once

constexpr int loyalty_card_identifier = 9999;

enum class PromotionType
{
    DISCOUNT, // price is lowever by price * DISCOUNT
    BULK      // for every n products, 1 is free
};
