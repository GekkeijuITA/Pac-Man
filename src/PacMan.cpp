#include "../includes/PacMan.hpp"
#include "../includes/textures.hpp"
#include "../includes/global_values.hpp"

#include <iostream>

PacMan::PacMan()
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

    sprite.setPosition({static_cast<float>(position.y * TILE_SIZE + (TILE_SIZE / 2)),
                        static_cast<float>((position.x + 3) * TILE_SIZE + (TILE_SIZE / 2))});

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

void PacMan::move(float elapsed)
{
    switch (direction)
    {
    case UP:
        fPosition.x -= speed * elapsed;
        break;
    case DOWN:
        fPosition.x += speed * elapsed;
        break;
    case LEFT:
        fPosition.y -= speed * elapsed;
        break;
    case RIGHT:
        fPosition.y += speed * elapsed;
        break;
    default:
        break;
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
}