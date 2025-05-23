#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <optional>

#include "global_values.hpp"
#include "textures.hpp"

struct TileData
{
    sf::Sprite sprite;
    char type;
};

struct TileFactory
{
    std::map<char, TileData> tiles;
    std::map<char, sf::Texture> textures;

    TileFactory();

private:
    void loadAll();
    void loadTile(char type, std::string texPath, int rotation = 0);
    void loadTile(char type, std::string texPath, sf::Vector2i texRect, int tileSize, sf::Vector2f scale, int rotation = 0);

public:
    std::optional<TileData> getTile(char type);
    static TileFactory &getIstance();
    void cleanUp();
};