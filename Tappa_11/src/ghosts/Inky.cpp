#include "../../includes/ghosts/Inky.hpp"

Inky::Inky(GameState &gameState) : Ghost(IN_HOUSE, 30, /*sf::IntRect({(MAP_WIDTH * TILE_SIZE) / 2, ((MAP_HEIGHT * TILE_SIZE) / 2) + (3 * TILE_SIZE)}, {(MAP_WIDTH * TILE_SIZE) / 2, (MAP_HEIGHT * TILE_SIZE) / 2}),*/ "Inky", gameState, {{RIGHT, INKY_R}, {LEFT, INKY_L}, {UP, INKY_U}, {DOWN, INKY_D}, {NONE, INKY_R}})
{
}