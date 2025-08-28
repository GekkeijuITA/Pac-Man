#pragma once
#include "../Fruit.hpp"

inline const sf::Vector2i STRAWBERRY_TEX = {3, 3};

struct Strawberry : public Fruit
{
    Strawberry(sf::Vector2i position, GameState &gameState);
};