#include "../Ghost.hpp"

inline const sf::Vector2i BLINKY_R = {0, 4};
inline const sf::Vector2i BLINKY_L = {2, 4};
inline const sf::Vector2i BLINKY_U = {4, 4};
inline const sf::Vector2i BLINKY_D = {6, 4};

struct Blinky : public Ghost
{
    Blinky(PacMan &pacman);
};