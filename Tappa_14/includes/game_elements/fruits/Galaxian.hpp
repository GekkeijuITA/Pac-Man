#pragma once
#include "../Fruit.hpp"

inline const sf::Vector2i GALAXIAN_TEX = {7, 3};

struct Galaxian : public Fruit
{
    Galaxian(sf::Vector2i position, GameState &gameState);
};