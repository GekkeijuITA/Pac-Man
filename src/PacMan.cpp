#include "../includes/PacMan.hpp"
#include "../includes/textures.hpp"
#include <iostream>

PacMan::PacMan() : map(nullptr)
{
    speed = 5.f;
    direction = NONE;
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
    sprite.setScale({(float)TILE_SIZE / tex.getSize().x,
                     (float)TILE_SIZE / tex.getSize().y});

    sprite.setOrigin({tex.getSize().x / 2.f,
                      tex.getSize().y / 2.f});

    sprite.setPosition({static_cast<float>((position.y + fPosition.y) * TILE_SIZE + (TILE_SIZE / 2)),
                        static_cast<float>((position.x + fPosition.x + 3) * TILE_SIZE + (TILE_SIZE / 2))});

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
    direction = dir;
}

void PacMan::setMap(char (*newMap)[MAP_HEIGHT])
{
    map = newMap;
}

bool PacMan::isWall(int x, int y)
{
    if (!map)
        return false;

    return map[x][y] == LINE_H || map[x][y] == LINE_V || map[x][y] == CORNER_0 || map[x][y] == CORNER_90 || map[x][y] == CORNER_180 || map[x][y] == CORNER_270;
}

void PacMan::move(float elapsed)
{
    sf::Vector2f new_fPosition(0.f, 0.f);

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

    // Calcola la nuova posizione frazionaria potenziale
    sf::Vector2f predicted_fPosition = fPosition + new_fPosition;

    // Calcola la nuova posizione intera potenziale
    sf::Vector2i predicted_position = position;
    if (std::abs(predicted_fPosition.x) >= 1.0f)
    {
        predicted_position.x += static_cast<int>(predicted_fPosition.x);
        predicted_fPosition.x -= static_cast<int>(predicted_fPosition.x);
    }
    if (std::abs(predicted_fPosition.y) >= 1.0f)
    {
        predicted_position.y += static_cast<int>(predicted_fPosition.y);
        predicted_fPosition.y -= static_cast<int>(predicted_fPosition.y);
    }

    sf::Vector2f hitboxOffset(0.15f, 0.15f);
    sf::Vector2f checkPoints[4];
    checkPoints[0] = {predicted_position.y + predicted_fPosition.y + hitboxOffset.y, predicted_position.x + predicted_fPosition.x + hitboxOffset.x};               // Top-left
    checkPoints[1] = {predicted_position.y + predicted_fPosition.y + hitboxOffset.y, predicted_position.x + predicted_fPosition.x + 1.0f - hitboxOffset.x};        // Top-right
    checkPoints[2] = {predicted_position.y + predicted_fPosition.y + 1.0f - hitboxOffset.y, predicted_position.x + predicted_fPosition.x + hitboxOffset.x};        // Bottom-left
    checkPoints[3] = {predicted_position.y + predicted_fPosition.y + 1.0f - hitboxOffset.y, predicted_position.x + predicted_fPosition.x + 1.0f - hitboxOffset.x}; // Bottom-right

    bool collision = false;
    for (const auto &point : checkPoints)
    {
        if (isWall(point.y, point.x))
        {
            collision = true;
            break;
        }
    }
    if (!collision)
    {
        fPosition += new_fPosition;
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
    
    sf::Vector2f currentCheckPoints[4];
    currentCheckPoints[0] = {position.y + fPosition.y + hitboxOffset.y, position.x + fPosition.x + hitboxOffset.x};
    currentCheckPoints[1] = {position.y + fPosition.y + hitboxOffset.y, position.x + fPosition.x + 1.0f - hitboxOffset.x};
    currentCheckPoints[2] = {position.y + fPosition.y + 1.0f - hitboxOffset.y, position.x + fPosition.x + hitboxOffset.x};
    currentCheckPoints[3] = {position.y + fPosition.y + 1.0f - hitboxOffset.y, position.x + fPosition.x + 1.0f - hitboxOffset.x};

    for (const auto& point : currentCheckPoints)
    {
        eat(static_cast<int>(point.y), static_cast<int>(point.x));
    }
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