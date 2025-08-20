#pragma once
#include "../Fruit.hpp"

inline const sf::Vector2i ORANGE_TEX = {4, 3};

struct Orange : public Fruit
{
    Orange(sf::Vector2i position, GameState &gameState);
};