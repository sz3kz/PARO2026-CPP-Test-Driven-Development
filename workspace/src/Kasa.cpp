#include "Kasa.hpp"
#include <algorithm>

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

void Cart::add(Registry const& registry, long identifier)
{
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
    return std::accumulate(
      contents.begin(),
      contents.end(),
      0.0,
      [registry](auto accumulator, auto identifier)
      { return accumulator + registry.contents.at(identifier).price; });
}

[[nodiscard]]
auto Cart::getEntryCount() const -> size_t
{
    return contents.size();
}

void Cart::close()
{
    for (auto identifier : contents)
    {
        del(identifier);
    }
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
