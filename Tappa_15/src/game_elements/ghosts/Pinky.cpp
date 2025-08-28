#include "../../../includes/game_elements/ghosts/Pinky.hpp"
#include "../../../includes/game_elements/GameState.hpp"

Pinky::Pinky(GameState &gameState)
    : Ghost(IN_HOUSE,
            0,
            sf::IntRect({0, 0}, {MAP_WIDTH / 2, MAP_HEIGHT / 2}),
            "Pinky",
            gameState, {{RIGHT, PINKY_R}, {LEFT, PINKY_L}, {UP, PINKY_U}, {DOWN, PINKY_D}, {NONE, PINKY_R}})
{
}

void Pinky::behaviour()
{
    enteredHouse = false;
    isTransitioning = false;

    Direction pacmanDir = getPacmanDirection();
    sf::Vector2i pacmanPos = getPacmanPosition();

    sf::Vector2i directionVector = fromDirectionToVector(pacmanDir);

    sf::Vector2i newTarget = pacmanPos + directionVector * 4;
    newTarget.x = std::clamp(newTarget.x, 0, static_cast<int>(map->size()) - 1);
    newTarget.y = std::clamp(newTarget.y, 0, static_cast<int>((*map)[0].size()) - 1);

    if (isAlignedToCell(0.09f))
    {
        if (targetTile != newTarget)
        {
            targetTile = newTarget;
            findPathBFS(targetTile);
        }
    }
}

void Pinky::respawn()
{
    Ghost::respawn();
}