#include "../../includes/ghosts/Pinky.hpp"

Pinky::Pinky() : Ghost(IN_HOUSE, 0.f)
{
    GHOST_TEX_MAP = {
        {RIGHT, PINKY_R},
        {LEFT, PINKY_L},
        {UP, PINKY_U},
        {DOWN, PINKY_D},
        {NONE, PINKY_R}};
}