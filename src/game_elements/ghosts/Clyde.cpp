#include "../../../includes/game_elements/ghosts/Clyde.hpp"

Clyde::Clyde(GameState &gameState) : Ghost(IN_HOUSE, 60, /*sf::IntRect({0, ((MAP_HEIGHT * TILE_SIZE) / 2) + (3 * TILE_SIZE)}, {(MAP_WIDTH * TILE_SIZE) / 2, (MAP_HEIGHT * TILE_SIZE) / 2}),*/ "Clyde", gameState, {{RIGHT, CLYDE_R}, {LEFT, CLYDE_L}, {UP, CLYDE_U}, {DOWN, CLYDE_D}, {NONE, CLYDE_R}})
{
}