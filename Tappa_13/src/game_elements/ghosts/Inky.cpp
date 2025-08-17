#include "../../../includes/game_elements/ghosts/Inky.hpp"
#include "../../../includes/game_elements/GameState.hpp"

Inky::Inky(GameState &gameState) : Ghost(IN_HOUSE, 30, sf::IntRect({MAP_WIDTH / 2, (MAP_HEIGHT / 2) + 3}, {MAP_WIDTH / 2, MAP_HEIGHT / 2}), "Inky", gameState, {{RIGHT, INKY_R}, {LEFT, INKY_L}, {UP, INKY_U}, {DOWN, INKY_D}, {NONE, INKY_R}})
{
}

void Inky::behaviour()
{
    enteredHouse = false;
    isTransitioning = false;

    sf::Vector2i pacmanPos = getPacmanPosition();
    sf::Vector2i pacmanDir = fromDirectionToVector(getPacmanDirection());
    sf::Vector2i newTarget;
    sf::Vector2i pacmanOffset = pacmanPos + pacmanDir * 2;

    if (gameState.blinky.isSpawned() && gameState.blinky.state == CHASE)
    {
        sf::Vector2i blinkyPos = gameState.blinky.getPosition();
        newTarget = pacmanOffset + 2 * (pacmanOffset - blinkyPos);
    }
    else
    {
        newTarget = pacmanOffset;
    }

    newTarget.x = std::clamp(newTarget.x, 0, static_cast<int>(map->size()) - 1);
    newTarget.y = std::clamp(newTarget.y, 0, static_cast<int>((*map)[0].size()) - 1);

    if (isAlignedToCell(0.09f) && (targetTile != newTarget || path.empty()))
    {
        targetTile = newTarget;
        findPathBFS(targetTile);
    }
}

void Inky::respawn()
{
    Ghost::respawn();
}