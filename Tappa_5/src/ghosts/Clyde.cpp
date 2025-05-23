#include "../../includes/ghosts/Clyde.hpp"

Clyde::Clyde(PacMan &pacmanRef) : Ghost(IN_HOUSE, 60, pacmanRef) {
    GHOST_TEX_MAP = {
        {RIGHT, CLYDE_R},
        {LEFT, CLYDE_L},
        {UP, CLYDE_U},
        {DOWN, CLYDE_D},
        {NONE, CLYDE_R}
    };
}