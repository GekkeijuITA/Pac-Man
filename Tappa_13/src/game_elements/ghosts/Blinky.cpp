#include "../../../includes/game_elements/ghosts/Blinky.hpp"

Blinky::Blinky(GameState &gameState)
    : Ghost(CHASE,
            0,
            /*sf::IntRect({(MAP_WIDTH * TILE_SIZE) / 2, 3 * TILE_SIZE}, {(MAP_WIDTH * TILE_SIZE) / 2, (MAP_HEIGHT * TILE_SIZE) / 2}),*/
            "Blinky",
            gameState, {{RIGHT, BLINKY_R}, {LEFT, BLINKY_L}, {UP, BLINKY_U}, {DOWN, BLINKY_D}, {NONE, BLINKY_R}})
{
}

void Blinky::move(float elapsed)
{
    if (!map || stoppedForScore || this == nullptr)
    {
        return;
    }

    if (state == EATEN)
    {
        Ghost::eatenState(elapsed);
    }
    else
    {
        Ghost::findPathBFS(getPacmanPosition());

        if (state == IN_HOUSE)
        {
            isTransitioning = false;
            enteredHouse = true;
        }
        else if (state == CHASE)
        {
            enteredHouse = false;
            isTransitioning = false;
            if (!path.empty())
            {
                sf::Vector2i nextTile = path.back();
                computeNextDirection(nextTile);

                if (position == nextTile)
                {
                    path.pop_back();
                }
            }
        }
        else
        {
            isTransitioning = false;
            enteredHouse = false;
        }
    }
}