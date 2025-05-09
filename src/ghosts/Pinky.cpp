#include "../../includes/ghosts/Pinky.hpp"

Pinky::Pinky(PacMan &pacmanRef) : Ghost(IN_HOUSE, 0, pacmanRef, /*sf::IntRect({0, 3 * TILE_SIZE}, {(MAP_WIDTH * TILE_SIZE) / 2, (MAP_HEIGHT * TILE_SIZE) / 2}),*/ "Pinky")
{
    GHOST_TEX_MAP = {
        {RIGHT, PINKY_R},
        {LEFT, PINKY_L},
        {UP, PINKY_U},
        {DOWN, PINKY_D},
        {NONE, PINKY_R}};
}