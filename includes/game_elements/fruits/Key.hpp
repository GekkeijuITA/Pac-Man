#pragma once
#include "../Fruit.hpp"

inline const sf::Vector2i KEY_TEX = {9, 3};

struct Key : public Fruit
{
    Key(sf::Vector2i position);
};