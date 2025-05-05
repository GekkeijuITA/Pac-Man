#include "../../includes/ghosts/Blinky.hpp"

Blinky::Blinky() {
    GHOST_TEX_MAP = {
        {RIGHT, BLINKY_R},
        {LEFT, BLINKY_L},
        {UP, BLINKY_U},
        {DOWN, BLINKY_D},
    };
}