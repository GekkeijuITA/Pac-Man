#include "../../includes/ghosts/Clyde.hpp"

Clyde::Clyde() : Ghost(IN_HOUSE, 3.f) {
    GHOST_TEX_MAP = {
        {RIGHT, CLYDE_R},
        {LEFT, CLYDE_L},
        {UP, CLYDE_U},
        {DOWN, CLYDE_D},
        {NONE, CLYDE_R}
    };
}