#include "Kasa.hpp"
#include <algorithm>

void registerProduct(Registry& registry, struct Product const& product)
{
    if (std::any_of(registry.begin(),
                    registry.end(),
                    [product](auto temp_product)
                    { return temp_product.id == product.id; }))
        return;
    registry.push_back(product);
}

void deregisterProduct(Registry& registry, double id)
{
    auto iterator =
      std::find_if(registry.begin(),
                   registry.end(),
                   [id](auto temp_product) { return temp_product.id == id; });
    if (iterator == registry.end())
    {
        return;
    }
    registry.erase(iterator);
}

void cartAddProduct(Registry const& registry, Cart& cart, double id)
{
    auto iterator =
      std::find_if(registry.begin(),
                   registry.end(),
                   [id](auto temp_product) { return temp_product.id == id; });
    if (iterator == registry.end())
    {
        return;
    }
    cart.push_back(id);
}

void cartDeleteProduct(Cart& cart, double id)
{
    auto iterator =
      std::find_if(cart.begin(),
                   cart.end(),
                   [id](auto current_id) { return current_id == id; });
    if (iterator == cart.end())
    {
        return;
    }
    cart.erase(iterator);
}
