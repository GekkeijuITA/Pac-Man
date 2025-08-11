#pragma once
#include "../Ghost.hpp"

inline const sf::Vector2i INKY_R = {0, 6};
inline const sf::Vector2i INKY_L = {2, 6};
inline const sf::Vector2i INKY_U = {4, 6};
inline const sf::Vector2i INKY_D = {6, 6};

struct Inky : public Ghost
{
    Inky(GameState &gameState);
};