#include "../../../includes/game_elements/ghosts/Blinky.hpp"

Blinky::Blinky(GameState &gameState)
    : Ghost(NORMAL,
            0,
            /*sf::IntRect({(MAP_WIDTH * TILE_SIZE) / 2, 3 * TILE_SIZE}, {(MAP_WIDTH * TILE_SIZE) / 2, (MAP_HEIGHT * TILE_SIZE) / 2}),*/
            "Blinky",
            gameState, {{RIGHT, BLINKY_R}, {LEFT, BLINKY_L}, {UP, BLINKY_U}, {DOWN, BLINKY_D}, {NONE, BLINKY_R}})
{
}