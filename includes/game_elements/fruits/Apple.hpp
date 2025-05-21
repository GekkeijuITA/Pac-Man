#pragma once
#include "../Fruit.hpp"

inline const sf::Vector2i APPLE_TEX = {5, 3};

struct Apple : public Fruit
{
    Apple(sf::Vector2i position);
};