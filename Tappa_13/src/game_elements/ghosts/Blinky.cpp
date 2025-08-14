#include "../../../includes/game_elements/ghosts/Blinky.hpp"
#include <iostream>

Blinky::Blinky(GameState &gameState)
    : Ghost(SCATTER,
            0,
            sf::IntRect({MAP_WIDTH  / 2, 0}, {MAP_WIDTH / 2, MAP_HEIGHT / 2}),
            "Blinky",
            gameState, {{RIGHT, BLINKY_R}, {LEFT, BLINKY_L}, {UP, BLINKY_U}, {DOWN, BLINKY_D}, {NONE, BLINKY_R}})
{
}

void Blinky::behaviour()
{
    if (state == IN_HOUSE)
    {
        exitHouse();
    }
    else if (state == CHASE)
    {
        enteredHouse = false;
        isTransitioning = false;

        static sf::Vector2i lastPacmanPosition = {-1, -1};
        sf::Vector2i pacmanPosition = getPacmanPosition();

        static sf::Vector2i lastTile = {-1, -1};

        if ((isAlignedToCell() || path.empty()) && (lastPacmanPosition != pacmanPosition || lastTile != position))
        {
            Ghost::findPathBFS(pacmanPosition);
            lastPacmanPosition = pacmanPosition;
            lastTile = position;
        }
    }
    else
    {
        isTransitioning = false;
        enteredHouse = false;
    }
}

void Blinky::respawn()
{
    Ghost::respawn();
    setState(CHASE);
}
