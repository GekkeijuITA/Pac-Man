#include "../includes/Ghost.hpp"
#include "../includes/textures.hpp"
#include <iostream>

Ghost::Ghost() : map(nullptr)
{
    speed = 2.5f;
    direction = NONE;
    lastDirection = NONE;

    if (!tex.loadFromFile(ASSET))
    {
        std::cerr << "Errore nel caricamento della texture di PacMan" << std::endl;
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
}

void Ghost::setDirection(Direction dir)
{
    direction = dir;
}

bool Ghost::isWall(int x, int y)
{
    if (!map)
        return false;

    return (*map)[x][y] == LINE_H || (*map)[x][y] == LINE_V || (*map)[x][y] == CORNER_0 || (*map)[x][y] == CORNER_90 || (*map)[x][y] == CORNER_180 || (*map)[x][y] == CORNER_270 || (*map)[x][y] == BLINKY || (*map)[x][y] == PINKY || (*map)[x][y] == INKY || (*map)[x][y] == CLYDE;
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
    if (lastDirection != RIGHT && !isWall(x, y - 1))
        possibleDirections.push_back(LEFT);
    if (lastDirection != LEFT && !isWall(x, y + 1))
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

void Ghost::move(float elapsed)
{
    // Prova a continuare nella direzione corrente prima di cambiare
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

    // Se la prossima tile è valida, continua
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
        // Se c'è un muro, scegli nuova direzione
        chooseDirection();
    }

    // Aggiorna la posizione intera quando supera 1.0
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