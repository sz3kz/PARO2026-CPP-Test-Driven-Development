#pragma once
#include "Registry.hpp"
#include <vector>

struct Cart
{
    std::vector<long> contents;
    void add(Registry& registry, long identifier);
    void del(long identifier);
    [[nodiscard]] auto calculateValue(Registry const& registry) const -> double;
    [[nodiscard]] auto getEntryCount() const -> size_t;
    void close(Registry& registry);
    void print(Registry const& registry) const;
};
