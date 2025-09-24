#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <optional>

#include "../lib/global_values.hpp"
#include "../lib/textures.hpp"
#include "../game_elements/fruits/Cherry.hpp"
#include "../game_elements/fruits/Strawberry.hpp"
#include "../game_elements/fruits/Orange.hpp"
#include "../game_elements/fruits/Apple.hpp"
#include "../game_elements/fruits/Grape.hpp"
#include "../game_elements/fruits/Galaxian.hpp"
#include "../game_elements/fruits/Bell.hpp"
#include "../game_elements/fruits/Key.hpp"

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