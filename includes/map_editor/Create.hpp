#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>

#include "../../includes/lib/text_ui.hpp"
#include "../../includes/GameMenu.hpp"

struct Tile
{
    std::string name;
    sf::Sprite sprite;
    sf::Vector2f position;
};

struct Create
{
    sf::RenderWindow &window;
    sf::Vector2i cursorPos;
    sf::Texture wallTex, cornerTex, pacdotTex, ghostDoorTex, gameAsset;
    std::map<char, Tile> tileSprites;
    std::vector<char> tileDisplayOrder;
    char lastTileType = EMPTY_BLOCK;
    bool optionsMenu = false, writingNameMap = false;
    int textCursorPos = 0;

    int hoveredTileIndex = -1, selectedTileIndex = -1, maxPacman = 1, maxBlinky = 1, maxInky = 1, maxPinky = 1, maxClyde = 1;
    ArcadeText arcadeText;
    std::vector<std::vector<char>> map;
    std::string mapName;

    GameMenu menu;
    std::vector<MenuOption> options;

    Create(sf::RenderWindow &window);

public:
    void doGraphics();
    void doUI();
    void setCursorPos(int x, int y);
    void handle(const sf::Event::MouseButtonPressed &mouseButton);
    void handle(const sf::Event::KeyPressed &key);
    void handle(const sf::Event::TextEntered &textEntered);

private:
    void drawCursor();
    bool isCursorOnMap();
    void drawMap();
    void drawInputText();
};