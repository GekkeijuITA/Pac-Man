#include "../includes/Ghost.hpp"
#include "../includes/textures.hpp"
#include <iostream>
#include <climits>

Ghost::Ghost(GhostState state, int dotLimit, PacMan &pacmanRef, sf::IntRect preferredZone, std::string name) : map(nullptr), state(state), dotLimit(dotLimit), pacman(pacmanRef), preferredZone(preferredZone), name(name)
{
    speed = 2.5f;
    direction = NONE;
    lastDirection = NONE;
    goingToZone = false;

    if (!tex.loadFromFile(ASSET))
    {
        std::cerr << "Errore nel caricamento della texture del fantasma" << std::endl;
        exit(1);
    }
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
    sf::RectangleShape rect(static_cast<sf::Vector2f>(preferredZone.size));
    rect.setPosition(static_cast<sf::Vector2f>(preferredZone.position));
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

bool Ghost::isInsideZone(int x, int y)
{
    return preferredZone.contains({x, y});
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

    // logica per la zona preferita del fantasma
    if (state == NORMAL && !possibleDirections.empty() && goingToZone)
    {
        sf::Vector2i center = preferredZone.getCenter();

        if (position.x < center.x && !isWall(position.x + 1, position.y))
            direction = DOWN;
        else if (position.x > center.x && !isWall(position.x - 1, position.y))
            direction = UP;
        else if (position.y < center.y && !isWall(position.x, position.y + 1))
            direction = RIGHT;
        else if (position.y > center.y && !isWall(position.x, position.y - 1))
            direction = LEFT;

        /*std::pair<Direction, int> bestDirection = {NONE, INT_MAX};

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
                int dist = std::abs(center.x - nextTile.x) + std::abs(center.y - nextTile.y);
                if (dist < bestDirection.second)
                {
                    bestDirection = {dir, dist};
                }
            }
        }

        if (bestDirection.first != NONE)
        {
            direction = bestDirection.first;
            lastDirection = direction;
            return;
        }*/
    }

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
        if (pacman.getDotEaten() >= dotLimit)
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
                goingToZone = false;
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

    if (alignedToCell)
    {
        int available = 0;
        if (!isWall(position.x - 1, position.y))
            available++;
        if (!isWall(position.x + 1, position.y))
            available++;
        if (!isWall(position.x, position.y - 1))
            available++;
        if (!isWall(position.x, position.y + 1))
            available++;

        if (available > 1)
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
    else
    {
        chooseDirection();
    }

    if (std::abs(fPosition.x) >= 1.0f)
    {
        position.x += static_cast<int>(fPosition.x);
        fPosition.x -= static_cast<int>(fPosition.x);
    }
    if (std::abs(fPosition.y) >= 1.0f)
    {
        position.y += static_cast<int>(fPosition.y);
        fPosition.y -= static_cast<int>(fPosition.y);
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