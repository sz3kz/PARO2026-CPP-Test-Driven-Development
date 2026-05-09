#pragma once
#include "World.hpp"
#include <iostream>

struct Promotion
{
    long identifier{ 0 };
    PromotionType type;
    double discount{ 0.0 };
    int nth_free{ 0 };
    bool is_active{ false };
};
