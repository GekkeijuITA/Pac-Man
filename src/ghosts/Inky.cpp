#include "../../includes/ghosts/Inky.hpp"

Inky::Inky(PacMan &pacmanRef, unsigned int w, unsigned int h) : Ghost(IN_HOUSE, 30, pacmanRef, sf::IntRect({(MAP_WIDTH * TILE_SIZE) / 2, ((MAP_HEIGHT * TILE_SIZE) / 2) + (3 * TILE_SIZE)}, {(MAP_WIDTH * TILE_SIZE) / 2, (MAP_HEIGHT * TILE_SIZE) / 2}), "Inky")
{
    GHOST_TEX_MAP = {
        {RIGHT, INKY_R},
        {LEFT, INKY_L},
        {UP, INKY_U},
        {DOWN, INKY_D},
        {NONE, INKY_R}};
}