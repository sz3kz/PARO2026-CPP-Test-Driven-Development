#pragma once
#include "Kasa.hpp"
#include <iostream>

struct Promotion
{
    long identity{ 0 };
    PromotionType type;
    double discount{ 0.0 };
    int nth_free{ 0 };
    bool is_active{ false };
    friend auto operator<<(std::ostream& output_stream,
                           struct Promotion const& promotion) -> std::ostream&;
};
