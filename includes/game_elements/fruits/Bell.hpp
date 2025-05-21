#pragma once
#include "../Fruit.hpp"

inline const sf::Vector2i BELL_TEX = {8, 3};

struct Bell : public Fruit
{
    Bell(sf::Vector2i position);
};