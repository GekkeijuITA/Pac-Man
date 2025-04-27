#include "../includes/PacMan.hpp"
#include "../includes/textures.hpp"
#include "../includes/global_values.hpp"

#include <iostream>

PacMan::PacMan()
{
    speed = 0.1f;
    position = {23, 13};
    direction = 0;
    if (!tex.loadFromFile(PACMAN))
    {
        std::cerr << "Errore nel caricamento della texture di PacMan" << std::endl;
        exit(1);
    }
    sf::Vector2u texSize = tex.getSize();
    //sprite.setScale({(float)TILE_SIZE / texSize.x, (float)TILE_SIZE / texSize.y});
    //sprite.setOrigin({texSize.x / 2.f, texSize.y / 2.f});
}

void PacMan::draw(sf::RenderWindow &window)
{
    sf::Sprite sprite(tex);
    sprite.setScale({(float)TILE_SIZE / tex.getSize().x, (float)TILE_SIZE / tex.getSize().y});
    sprite.setOrigin({tex.getSize().x / 2.f, tex.getSize().y / 2.f});
    sprite.setPosition({(float) position.y * TILE_SIZE + (TILE_SIZE/2),(float) (position.x + 3) * TILE_SIZE + (TILE_SIZE/2)});
    sprite.setRotation(sf::degrees(90.f * direction));
    window.draw(sprite);

    /*sf::RectangleShape debugPacman({TILE_SIZE, TILE_SIZE});
    debugPacman.setOutlineColor(sf::Color::Yellow);
    debugPacman.setOutlineThickness(1);
    debugPacman.setFillColor(sf::Color::Transparent);
    debugPacman.setPosition({(float) position.y * TILE_SIZE,(float) (position.x + 3) * TILE_SIZE});
    window.draw(debugPacman);*/
}