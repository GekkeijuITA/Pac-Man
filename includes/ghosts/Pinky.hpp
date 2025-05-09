#include "../Ghost.hpp"

inline const sf::Vector2i PINKY_R = {0, 5};
inline const sf::Vector2i PINKY_L = {2, 5};
inline const sf::Vector2i PINKY_U = {4, 5};
inline const sf::Vector2i PINKY_D = {6, 5};

struct Pinky : public Ghost
{
    Pinky(State &gameState);
};