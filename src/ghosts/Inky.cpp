#include "../../includes/ghosts/Inky.hpp"

Inky::Inky() {
    GHOST_TEX_MAP = {
        {RIGHT, INKY_R},
        {LEFT, INKY_L},
        {UP, INKY_U},
        {DOWN, INKY_D},
    };
}