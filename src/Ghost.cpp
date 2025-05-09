#include "../includes/Ghost.hpp"
#include "../includes/State.hpp"
#include "../includes/textures.hpp"
#include <iostream>
#include <climits>
#include <algorithm>

#define COLLIDE_BOX 1.5f

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

    if (!tex.loadFromFile(ASSET))
    {
        std::cerr << "Errore nel caricamento della texture del fantasma" << std::endl;
        exit(1);
    }

    /*if (name == "Blinky")
    {
        std::cout << "====== " << name << " ======" << std::endl;
        std::cout << "Prefered zone: " << preferredZone.size.x << ", " << preferredZone.size.y << std::endl;
        std::cout << "Prefered zone: " << preferredZone.position.x << ", " << preferredZone.position.y << std::endl;
    }*/
}

void Ghost::draw(sf::RenderWindow &window)
{
    sf::Vector2i ghostPos = GHOST_TEX_MAP.at(direction);
    sf::Sprite sprite = createSprite(tex, ghostPos, {2.f, 2.f}, 1.5f, TILE_SIZE / 2, true);

    float y = static_cast<float>((fPosition.x + position.x + 3 + 0.5f) * TILE_SIZE);
    float x = static_cast<float>((fPosition.y + position.y + 0.5f) * TILE_SIZE);

    sprite.setPosition({x, y});

    window.draw(sprite);


    //  DEBUG
    sf::RectangleShape rect({COLLIDE_BOX * TILE_SIZE, COLLIDE_BOX * TILE_SIZE});
    rect.setPosition({x,y});
    rect.setOrigin({COLLIDE_BOX * TILE_SIZE / 2, COLLIDE_BOX * TILE_SIZE / 2});
    sf::Color color;
    if (name == "Blinky")
    {
        color = sf::Color::Red;
    }
    else if (name == "Pinky")
    {
        color = sf::Color::Magenta;
    }
    else if (name == "Inky")
    {
        color = sf::Color::Cyan;
    }
    else if (name == "Clyde")
    {
        color = sf::Color::Yellow;
    }
    rect.setOutlineThickness(2);
    rect.setOutlineColor(color);
    rect.setFillColor(sf::Color::Transparent);
    window.draw(rect);

    /*rect.setPosition(static_cast<sf::Vector2f>(preferredZone.getCenter()));
    rect.setSize({TILE_SIZE, TILE_SIZE});
    rect.setFillColor(color);
    rect.setOutlineThickness(0);
    window.draw(rect);*/
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

/*bool Ghost::isInsideZone(int x, int y)
{
    // std::cout << "Ghost " << name << " isInsideZone: " << x << ", " << y << std::endl;
    return preferredZone.contains({x, y});
}*/

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

    // logica per la zona preferita del fantasma
    /*if (state == NORMAL && !possibleDirections.empty())
    {
        std::pair<Direction, int> bestDirection = {NONE, INT_MAX};
        sf::Vector2i center = preferredZone.getCenter();

        for (Direction dir : possibleDirections)
        {
            sf::Vector2i nextTile = position;

            switch (dir)
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
                // Distanza di Manhattan
                int dist = std::abs((nextTile.x * TILE_SIZE) - center.x) + std::abs((nextTile.y * TILE_SIZE) - center.y);

                if (isInsideZone(nextTile.x * TILE_SIZE, nextTile.y * TILE_SIZE))
                {
                    if (dist < bestDirection.second)
                    {
                        bestDirection = {dir, dist};
                    }
                }
            }
        }

        if (bestDirection.first != NONE)
        {
            direction = bestDirection.first;
            lastDirection = direction;
            return;
        }
    }*/

    if (!possibleDirections.empty())
    {
        direction = possibleDirections[rand() % possibleDirections.size()];
        lastDirection = direction;
    }
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

    if (state == NORMAL)
    {
        eat(position.x, position.y);
    }
}

void Ghost::setState(GhostState newState)
{
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
    if (map == nullptr)
        return;

    sf::Vector2i distanceFromPacman = gameState.pacman.position - position;
    float distance = std::sqrt(distanceFromPacman.x * distanceFromPacman.x + distanceFromPacman.y * distanceFromPacman.y);

    if (distance < COLLIDE_BOX)
    {
        gameState.lives--;
        gameState.resetRound();
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