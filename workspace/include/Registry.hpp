#pragma once
#include "Kasa.hpp"
#include "Product.hpp"
#include "Promotion.hpp"
#include <map>

struct Registry
{
    std::map<long, Product> contents;
    std::map<long, Promotion> promotions;
    bool loyalty_card_active{ false };
    void add(struct Product const& product);
    void activate_loyalty_card();
    void deactivate_loyalty_card();
    void add_promotion(long identity, double discount);
    void add_promotion(long identity, int nth_free);
    void activate_promotion(long identity);
    void deactivate_promotion(long identity);
    void update_promotion_status();
    void del(long identifier);
    [[nodiscard]] auto getEntryCount() const -> size_t;
    void print() const;
    void print_promotions() const;
};
