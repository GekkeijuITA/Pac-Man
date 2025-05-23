#include "../../includes/ghosts/Blinky.hpp"

Blinky::Blinky(PacMan &pacmanRef) : Ghost(NORMAL, 0, pacmanRef) {
    GHOST_TEX_MAP = {
        {RIGHT, BLINKY_R},
        {LEFT, BLINKY_L},
        {UP, BLINKY_U},
        {DOWN, BLINKY_D},
        {NONE, BLINKY_R}
    };
}