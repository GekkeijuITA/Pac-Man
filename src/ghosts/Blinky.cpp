#include "../../includes/ghosts/Blinky.hpp"

Blinky::Blinky(PacMan &pacmanRef, int w, int h) : Ghost(NORMAL, 0, pacmanRef, sf::IntRect({(MAP_WIDTH * TILE_SIZE) / 2, 3 * TILE_SIZE}, {(MAP_WIDTH * TILE_SIZE) / 2, (MAP_HEIGHT * TILE_SIZE) / 2}), "Blinky")
{
    GHOST_TEX_MAP = {
        {RIGHT, BLINKY_R},
        {LEFT, BLINKY_L},
        {UP, BLINKY_U},
        {DOWN, BLINKY_D},
        {NONE, BLINKY_R}};
}