#include "../includes/Ghost.hpp"
#include "../includes/State.hpp"
#include "../includes/textures.hpp"
#include <iostream>
#include <climits>
#include <algorithm>
#include <queue>
#include <array>
#include <unordered_map>

#define COLLIDE_BOX 1.5f

namespace std
{
    template <>
    struct hash<sf::Vector2i>
    {
        size_t operator()(const sf::Vector2i &vector) const noexcept
        {
            size_t h1 = std::hash<int>{}(vector.x);
            size_t h2 = std::hash<int>{}(vector.y);
            return h1 ^ (h2 << 1);
        }
    };
}

Ghost::Ghost(
    GhostState state,
    int dotLimit,
    /*sf::IntRect preferredZone,*/
    std::string name,
    State &gameState) : map(nullptr),
                        state(state),
                        dotLimit(dotLimit),
                        /*preferredZone(preferredZone),*/
                        name(name),
                        gameState(gameState)
{
    speed = 2.5f;
    direction = NONE;
    lastDirection = NONE;
    isTransitioning = false;
    isGoingToExit = false;
    isGoingToSpawn = false;

    if (!tex.loadFromFile(ASSET))
    {
        std::cerr << "Errore nel caricamento della texture del fantasma" << std::endl;
        exit(1);
    }
}

void Ghost::draw(sf::RenderWindow &window)
{
    sf::Vector2i ghostPos;
    if (state == EATEN)
    {
        ghostPos = GHOST_EYES_TEX_MAP.at(direction);
    }
    else if (state == SCARED)
    {
        ghostPos = GHOST_SCARED;
    }
    else
    {
        ghostPos = GHOST_TEX_MAP.at(direction);
    }
    sf::Sprite sprite = createSprite(tex, ghostPos, {2.f, 2.f}, 1.5f, TILE_SIZE / 2, true);

    float y = static_cast<float>((fPosition.x + position.x + 3 + 0.5f) * TILE_SIZE);
    float x = static_cast<float>((fPosition.y + position.y + 0.5f) * TILE_SIZE);

    sprite.setPosition({x, y});

    window.draw(sprite);
}

void Ghost::setPosition(int x, int y)
{
    position.x = x;
    position.y = y;
    fPosition = {0.f, 0.f};
}

void Ghost::setMap(std::vector<std::vector<char>> *newMap)
{
    map = newMap;
    nearestExitTile = getNearestExitTile();
}

void Ghost::setDirection(Direction dir)
{
    direction = dir;
}

bool Ghost::isWall(int x, int y)
{
    if (!map)
        return false;

    if (state == NORMAL && (*map)[x][y] == GHOST_DOOR)
        return true;

    return (*map)[x][y] == LINE_H || (*map)[x][y] == LINE_V || (*map)[x][y] == CORNER_0 || (*map)[x][y] == CORNER_90 || (*map)[x][y] == CORNER_180 || (*map)[x][y] == CORNER_270;
}

void Ghost::chooseDirection()
{
    std::vector<Direction> possibleDirections;
    int x = position.x;
    int y = position.y;

    if (lastDirection != DOWN && !isWall(x - 1, y))
        possibleDirections.push_back(UP);
    if (lastDirection != UP && !isWall(x + 1, y))
        possibleDirections.push_back(DOWN);
    if (lastDirection != RIGHT && !isWall(x, y - 1) && state != IN_HOUSE)
        possibleDirections.push_back(LEFT);
    if (lastDirection != LEFT && !isWall(x, y + 1) && state != IN_HOUSE)
        possibleDirections.push_back(RIGHT);

    if (possibleDirections.empty())
    {
        if (!isWall(x - 1, y))
            possibleDirections.push_back(UP);
        if (!isWall(x + 1, y))
            possibleDirections.push_back(DOWN);
        if (!isWall(x, y - 1))
            possibleDirections.push_back(LEFT);
        if (!isWall(x, y + 1))
            possibleDirections.push_back(RIGHT);
    }

    if (!possibleDirections.empty())
    {
        direction = possibleDirections[rand() % possibleDirections.size()];
        lastDirection = direction;
    }
}

// https://medium.com/@RobuRishabh/classic-graph-algorithms-c-9773f2841f2e
std::vector<sf::Vector2i> Ghost::findPathBFS(sf::Vector2i destination)
{
    if (!map)
        return {};

    std::vector<sf::Vector2i> path;
    std::unordered_map<sf::Vector2i, bool> visited;
    std::unordered_map<sf::Vector2i, sf::Vector2i> parent;
    std::queue<sf::Vector2i> q;
    visited[position] = true;
    parent[position] = position;
    q.push(position);

    while (!q.empty())
    {
        sf::Vector2i current = q.front();
        q.pop();

        if (current == destination)
        {
            sf::Vector2i step = current;
            while (parent[step] != position)
            {
                path.push_back(step);
                step = parent[step];
            }
            path.push_back(position);
            std::reverse(path.begin(), path.end());

            // print the path
            std::cout << "Path from " << position.x << "," << position.y << " to " << destination.x << "," << destination.y << ": ";
            for (const auto &p : path)
            {
                std::cout << "(" << p.x << "," << p.y << ") ";
            }
            std::cout << std::endl;

            return path;
        }

        std::vector<sf::Vector2i> neighbours = {
            {current.x - 1, current.y},
            {current.x + 1, current.y},
            {current.x, current.y - 1},
            {current.x, current.y + 1}};

        for (sf::Vector2i neighbour : neighbours)
        {
            if (!visited[neighbour] &&
                neighbour.x >= 0 &&
                neighbour.x < map->size() &&
                neighbour.y >= 0 &&
                neighbour.y < (*map)[0].size() &&
                !isWall(neighbour.x, neighbour.y))
            {
                visited[neighbour] = true;
                parent[neighbour] = current;
                q.push(neighbour);
            }
        }
    }

    return {};
}

void Ghost::followPathTo(sf::Vector2i destination, bool &isGoing)
{
    if (isGoing)
        return;

    std::vector<sf::Vector2i> path = findPathBFS(destination);
    if (path.size() < 2)
    {
        return;
    }

    sf::Vector2i nextTarget = path[1];
    sf::Vector2i currentTile = position;

    if (nextTarget.x < currentTile.x)
        direction = UP;
    else if (nextTarget.x > currentTile.x)
        direction = DOWN;
    else if (nextTarget.y < currentTile.y)
        direction = LEFT;
    else if (nextTarget.y > currentTile.y)
        direction = RIGHT;

    lastDirection = direction;
    isGoing = true;
}

void Ghost::move(float elapsed)
{
    if (state == IN_HOUSE)
    {
        if (gameState.pacman.getDotEaten() >= dotLimit)
        {
            if (position != nearestExitTile)
            {
                if (position.x < nearestExitTile.x && !isWall(position.x + 1, position.y))
                    direction = DOWN;
                else if (position.x > nearestExitTile.x && !isWall(position.x - 1, position.y))
                    direction = UP;
                else if (position.y < nearestExitTile.y && !isWall(position.x, position.y + 1))
                    direction = RIGHT;
                else if (position.y > nearestExitTile.y && !isWall(position.x, position.y - 1))
                    direction = LEFT;

                lastDirection = direction;
            }
            else
            {
                state = NORMAL;
                isTransitioning = true;
            }
        }
        else
        {
            chooseDirection();
        }
    }
    else if (state == EATEN)
    {
        // Fare BFS per tornare a casa
        if (position != spawn)
        {
            if (position.x < spawn.x && !isWall(position.x + 1, position.y))
                direction = DOWN;
            else if (position.x > spawn.x && !isWall(position.x - 1, position.y))
                direction = UP;
            else if (position.y < spawn.y && !isWall(position.x, position.y + 1))
                direction = RIGHT;
            else if (position.y > spawn.y && !isWall(position.x, position.y - 1))
                direction = LEFT;

            lastDirection = direction;
        }
        else
        {
            state = IN_HOUSE;
            isTransitioning = true;
        }
    }

    bool alignedToCell = std::abs(fPosition.x) < 0.01f && std::abs(fPosition.y) < 0.01f;

    if (alignedToCell && !isTransitioning)
    {
        bool canMove = false;
        switch (direction)
        {
        case UP:
            canMove = !isWall(position.x - 1, position.y);
            break;
        case DOWN:
            canMove = !isWall(position.x + 1, position.y);
            break;
        case LEFT:
            canMove = !isWall(position.x, position.y - 1);
            break;
        case RIGHT:
            canMove = !isWall(position.x, position.y + 1);
            break;
        }

        if (!canMove)
        {
            chooseDirection();
        }
    }

    sf::Vector2i nextTile = position;

    switch (direction)
    {
    case UP:
        nextTile.x--;
        break;
    case DOWN:
        nextTile.x++;
        break;
    case LEFT:
        nextTile.y--;
        break;
    case RIGHT:
        nextTile.y++;
        break;
    default:
        break;
    }

    if (!isWall(nextTile.x, nextTile.y))
    {
        sf::Vector2f movement(0, 0);
        switch (direction)
        {
        case UP:
            movement.x = -speed * elapsed;
            break;
        case DOWN:
            movement.x = speed * elapsed;
            break;
        case LEFT:
            movement.y = -speed * elapsed;
            break;
        case RIGHT:
            movement.y = speed * elapsed;
            break;
        default:
            break;
        }
        fPosition += movement;
    }
    else if (!isTransitioning)
    {
        chooseDirection();
    }

    if (std::abs(fPosition.x) >= 1.0f)
    {
        position.x += static_cast<int>(fPosition.x);
        fPosition.x -= static_cast<int>(fPosition.x);

        if (isTransitioning && position != nearestExitTile)
        {
            isTransitioning = false;
        }
    }
    if (std::abs(fPosition.y) >= 1.0f)
    {
        position.y += static_cast<int>(fPosition.y);
        fPosition.y -= static_cast<int>(fPosition.y);

        if (isTransitioning && position != nearestExitTile)
        {
            isTransitioning = false;
        }
    }

    if (state != EATEN)
    {
        eat(position.x, position.y);
    }
}

void Ghost::setState(GhostState newState)
{
    if (state != newState)
        state = newState;
}

void Ghost::addExitTile(int x, int y)
{
    exitTiles.push_back({x, y});
}

sf::Vector2i Ghost::getNearestExitTile()
{
    if (exitTiles.empty())
    {
        std::cerr << "Nessun tile di uscita disponibile" << std::endl;
        return {0, 0};
    }

    sf::Vector2i nearest = exitTiles[0];
    int minDist = std::abs(position.x - nearest.x) + std::abs(position.y - nearest.y);

    for (const auto &tile : exitTiles)
    {
        int dist = std::abs(position.x - tile.x) + std::abs(position.y - tile.y);
        if (dist < minDist)
        {
            minDist = dist;
            nearest = tile;
        }
    }

    return nearest;
}

void Ghost::eat(int x, int y)
{
    sf::Vector2i distanceFromPacman = gameState.pacman.position - position;
    float distance = std::sqrt(distanceFromPacman.x * distanceFromPacman.x + distanceFromPacman.y * distanceFromPacman.y);

    if (distance < COLLIDE_BOX)
    {
        if (state == SCARED || state == RETURNING_FROM_SCARED)
        {
            setState(EATEN);
            speed *= 2;
            gameState.score += (200 * gameState.pacman.ghostStreak);
            gameState.pacman.ghostStreak++;
        }
        else
        {
            gameState.lives--;
            gameState.resetRound();
        }
    }
}

void Ghost::respawn(GhostState state)
{
    setState(state);
    setPosition(spawn.x, spawn.y);
    setDirection(NONE);
    lastDirection = NONE;
    isTransitioning = false;
}