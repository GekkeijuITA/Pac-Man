#include "../../../includes/game_elements/ghosts/Pinky.hpp"
#include "../../../includes/game_elements/GameState.hpp"

Pinky::Pinky(GameState &gameState) : Ghost(IN_HOUSE, 0, /*sf::IntRect({0, 3 * TILE_SIZE}, {(MAP_WIDTH * TILE_SIZE) / 2, (MAP_HEIGHT * TILE_SIZE) / 2}),*/ "Pinky", gameState, {{RIGHT, PINKY_R}, {LEFT, PINKY_L}, {UP, PINKY_U}, {DOWN, PINKY_D}, {NONE, PINKY_R}})
{
}