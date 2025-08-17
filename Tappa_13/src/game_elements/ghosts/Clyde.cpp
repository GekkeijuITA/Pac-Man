#include "../../../includes/game_elements/ghosts/Clyde.hpp"
#include "../../../includes/game_elements/GameState.hpp"

Clyde::Clyde(GameState &gameState) : Ghost(
                                         IN_HOUSE,
                                         60,
                                         sf::IntRect({0, (MAP_HEIGHT / 2) + 3}, {MAP_WIDTH / 2, MAP_HEIGHT / 2}),
                                         "Clyde",
                                         gameState, {{RIGHT, CLYDE_R}, {LEFT, CLYDE_L}, {UP, CLYDE_U}, {DOWN, CLYDE_D}, {NONE, CLYDE_R}})
{
}

void Clyde::behaviour()
{
    sf::Vector2i newTarget;
    double dist = distance(getPacmanPosition());

    if (dist >= 8)
    {
        newTarget = getPacmanPosition();
    }
    else
    {
        newTarget = {preferredAngle.position.y, preferredAngle.position.x};
    }

    if (isAlignedToCell(0.09f) && (targetTile != newTarget || path.empty()))
    {
        targetTile = newTarget;
        findPathBFS(targetTile);
    }
}

void Clyde::respawn()
{
    Ghost::respawn();
}