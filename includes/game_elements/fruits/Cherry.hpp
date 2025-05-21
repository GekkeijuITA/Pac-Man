#pragma once
#include "../Fruit.hpp"

inline const sf::Vector2i CHERRY_TEX = {2, 3};

struct Cherry : public Fruit
{
    Cherry(sf::Vector2i position);
};