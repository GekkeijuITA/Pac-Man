#include "../../includes/ghosts/Inky.hpp"

Inky::Inky() : Ghost(IN_HOUSE, 6.f)
{
    GHOST_TEX_MAP = {
        {RIGHT, INKY_R},
        {LEFT, INKY_L},
        {UP, INKY_U},
        {DOWN, INKY_D},
        {NONE, INKY_R}
    };
}