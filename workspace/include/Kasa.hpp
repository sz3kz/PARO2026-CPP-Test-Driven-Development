#pragma once
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

struct Product
{
    long identifier{ 0 };
    std::string name;
    double price{ 0 };

    Product() = default;

    Product(long product_identifier,
            std::string product_name,
            double product_price)
      : identifier(product_identifier)
      , name(std::move(product_name))
      , price(product_price) {};
    friend auto operator<<(std::ostream& output_stream,
                           struct Product const& product) -> std::ostream&;
};

struct Registry
{
    std::map<long, Product> contents;
    bool loyalty_card_active{ false };
    void add(struct Product const& product);
    void activate_loyalty_card();
    void deactivate_loyalty_card();
    void del(long identifier);
    [[nodiscard]] auto getEntryCount() const -> size_t;
    void print() const;
};

struct Cart
{
    std::vector<long> contents;
    void add(Registry const& registry, long identifier);
    void del(long identifier);
    [[nodiscard]] auto calculateValue(Registry const& registry) const -> double;
    [[nodiscard]] auto getEntryCount() const -> size_t;
    void close();
    void print(Registry const& registry) const;
};
