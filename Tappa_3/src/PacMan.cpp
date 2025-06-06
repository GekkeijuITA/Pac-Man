#include "../includes/PacMan.hpp"
#include "../includes/textures.hpp"
#include <iostream>

PacMan::PacMan() : map(nullptr)
{
    speed = 5.f;
    direction = NONE;
    nextDirection = NONE;

    if (!tex.loadFromFile(PACMAN))
    {
        std::cerr << "Errore nel caricamento della texture di PacMan" << std::endl;
        exit(1);
    }
    sf::Vector2u texSize = tex.getSize();
}

void PacMan::draw(sf::RenderWindow &window)
{
    sf::Sprite sprite(tex);
    sprite.setScale({(float)TILE_SIZE / (tex.getSize().x),
                     (float)TILE_SIZE / (tex.getSize().y)});

    sprite.setOrigin({tex.getSize().x / 2.f,
                      tex.getSize().y / 2.f});

    float y = static_cast<float>((fPosition.x + position.x + 3 + 0.5f) * TILE_SIZE);
    float x = static_cast<float>((fPosition.y + position.y + 0.5f) * TILE_SIZE);

    sprite.setPosition({x, y});

    sprite.setRotation(sf::degrees(90.f * direction));
    window.draw(sprite);
}

void PacMan::setPosition(int x, int y)
{
    position.x = x;
    position.y = y;
    fPosition = {0.f, 0.f};
}

void PacMan::setRotation(Direction dir)
{
    nextDirection = dir;
}

void PacMan::setMap(char (*newMap)[MAP_HEIGHT])
{
    map = newMap;
}

bool PacMan::isWall(int x, int y)
{
    if (!map)
        return false;

    return map[x][y] == LINE_H || map[x][y] == LINE_V || map[x][y] == CORNER_0 || map[x][y] == CORNER_90 || map[x][y] == CORNER_180 || map[x][y] == CORNER_270 || map[x][y] == GHOST_DOOR;
}

void PacMan::updateDirection()
{
    if (nextDirection != NONE && nextDirection != direction)
    {
        sf::Vector2i dirVec(0, 0);
        switch (nextDirection)
        {
        case UP:
            dirVec.x = -1;
            break;
        case DOWN:
            dirVec.x = 1;
            break;
        case LEFT:
            dirVec.y = -1;
            break;
        case RIGHT:
            dirVec.y = 1;
            break;
        default:
            break;
        }

        float alignmentThreshold = 0.2f;

        if (!isWall(position.x + dirVec.x, position.y + dirVec.y) &&
            std::abs(fPosition.x) < alignmentThreshold &&
            std::abs(fPosition.y) < alignmentThreshold)
        {
            direction = nextDirection;
            fPosition = {0.f, 0.f};
            nextDirection = NONE;
        }
    }
}

void PacMan::move(float elapsed)
{
    sf::Vector2f new_fPosition = fPosition;

    switch (direction)
    {
    case UP:
        new_fPosition.x -= speed * elapsed;
        break;
    case DOWN:
        new_fPosition.x += speed * elapsed;
        break;
    case LEFT:
        new_fPosition.y -= speed * elapsed;
        break;
    case RIGHT:
        new_fPosition.y += speed * elapsed;
        break;
    default:
        break;
    }

    sf::Vector2i newPosition = position + static_cast<sf::Vector2i>(new_fPosition);

    if (!isWall(newPosition.x, newPosition.y))
    {
        fPosition = new_fPosition;
    }

    if (std::abs(fPosition.x) >= 1.0f)
    {
        int move_x = static_cast<int>(fPosition.x);
        position.x += move_x;
        fPosition.x -= move_x;
    }

    if (std::abs(fPosition.y) >= 1.0f)
    {
        int move_y = static_cast<int>(fPosition.y);
        position.y += move_y;
        fPosition.y -= move_y;
    }

    eat(position.x, position.y);
}

void PacMan::eat(int x, int y)
{
    if (map[x][y] == PACDOT)
    {
        map[x][y] = EMPTY_BLOCK;
    }
    else if (map[x][y] == POWERPELLET)
    {
        map[x][y] = EMPTY_BLOCK;
    }
}