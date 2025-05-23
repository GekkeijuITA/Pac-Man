#include "../../includes/lib/TileFactory.hpp"
#include <iostream>

TileFactory::TileFactory()
{
    loadAll();
}

void TileFactory::loadTile(char type, std::string texPath, int rotation)
{
    sf::Texture tex;
    if (!tex.loadFromFile(texPath))
    {
        std::cerr << "Error loading straight line texture" << std::endl;
    }
    textures.insert({type, tex});

    sf::Sprite sprite(textures.at(type));
    sprite.setScale({(float)TILE_SIZE / tex.getSize().x, (float)TILE_SIZE / tex.getSize().y});
    sprite.setOrigin({sprite.getLocalBounds().size.x / 2.f, sprite.getLocalBounds().size.y / 2.f});

    if (rotation != 0)
    {
        sprite.setRotation(sf::degrees(rotation));
    }

    tiles.insert({type, {sprite, type}});
}

void TileFactory::loadTile(char type, std::string texPath, sf::Vector2i texRect, int tileSize, sf::Vector2f scale, int rotation)
{
    sf::Texture tex;
    if (!tex.loadFromFile(texPath))
    {
        std::cerr << "Error loading straight line texture" << std::endl;
    }
    textures.insert({type, tex});

    sf::IntRect rect({texRect.x * tileSize, texRect.y * tileSize}, {tileSize, tileSize});
    sf::Sprite sprite(textures.at(type), rect);
    sprite.setScale(scale);
    sprite.setOrigin({sprite.getLocalBounds().size.x / 2.f, sprite.getLocalBounds().size.y / 2.f});

    if (rotation != 0)
    {
        sprite.setRotation(sf::degrees(rotation));
    }

    tiles.insert({type, {sprite, type}});
}

void TileFactory::loadAll()
{
    loadTile(LINE_H, STRAIGHT_LINE_H);
    loadTile(LINE_V, STRAIGHT_LINE_H, 90);
    loadTile(CORNER_0, ANGLE_0);
    loadTile(CORNER_90, ANGLE_0, -90);
    loadTile(CORNER_180, ANGLE_0, -180);
    loadTile(CORNER_270, ANGLE_0, -270);
    loadTile(GHOST_DOOR_H, GHOST_DOOR_TEX_H);
    loadTile(GHOST_DOOR_V, GHOST_DOOR_TEX_H, 90);
    loadTile(PACDOT, PACDOT_TEX);
    loadTile(POWERPELLET, POWERPELLET_TEX);
    loadTile(PACMAN, ASSET, sf::Vector2i(1, 1), TILE_SIZE / 2, sf::Vector2f(2.f, 2.f));
    loadTile(FRUIT, ASSET, sf::Vector2i(2, 3), TILE_SIZE / 2, sf::Vector2f(2.f, 2.f));
    loadTile(BLINKY, ASSET, sf::Vector2i(2, 4), TILE_SIZE / 2, sf::Vector2f(2.f, 2.f));
    loadTile(PINKY, ASSET, sf::Vector2i(2, 5), TILE_SIZE / 2, sf::Vector2f(2.f, 2.f));
    loadTile(INKY, ASSET, sf::Vector2i(2, 6), TILE_SIZE / 2, sf::Vector2f(2.f, 2.f));
    loadTile(CLYDE, ASSET, sf::Vector2i(2, 7), TILE_SIZE / 2, sf::Vector2f(2.f, 2.f));
}

// Dato il carattere del tipo di tile, restituisce il TileData corrispondente (es. 'P' per PACMAN)
std::optional<TileData> TileFactory::getTile(char type)
{
    if (tiles.find(type) != tiles.end())
    {
        return tiles.at(type);
    }
    else
    {
        std::cerr << "Tile type not found: " << type << std::endl;
        return std::nullopt;
    }
}

TileFactory &TileFactory::getIstance()
{
    static TileFactory instance;
    return instance;
}

void TileFactory::cleanUp()
{
    tiles.clear();
    textures.clear();
}