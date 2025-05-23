#include "../Ghost.hpp"

inline const sf::Vector2i CLYDE_R = {0, 7};
inline const sf::Vector2i CLYDE_L = {2, 7};
inline const sf::Vector2i CLYDE_U = {4, 7};
inline const sf::Vector2i CLYDE_D = {6, 7};

struct Clyde : public Ghost
{
    Clyde(State &gameState);
};