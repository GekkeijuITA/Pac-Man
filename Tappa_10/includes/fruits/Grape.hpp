#pragma once
#include "../Fruit.hpp"

inline const sf::Vector2i GRAPE_TEX = {6, 3};

struct Grape : public Fruit
{
    Grape(sf::Vector2i position);
};