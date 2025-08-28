#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>

#include "../../includes/lib/text_ui.hpp"
#include "../../includes/GameMenu.hpp"
#include "MapEditor.hpp"

struct MapEditor;

struct Tile
{
    std::string name;
    sf::Sprite sprite;
    sf::Vector2f position;
};

struct Create
{
    sf::RenderWindow &window;
    MapEditor &mapEditor;

    sf::Vector2i cursorPos;
    sf::Texture wallTex, cornerTex, pacdotTex, ghostDoorTex, gameAsset;
    std::map<char, Tile> tileSprites;
    std::vector<char> tileDisplayOrder;
    char lastTileType = EMPTY_BLOCK;
    bool optionsMenu = false, writingNameMap = false;
    int textCursorPos = 0, pacdotPlaced = 0, ghostDoorPlaced = 0;

    int hoveredTileIndex = -1, selectedTileIndex = -1, maxPacman = 1, maxBlinky = 1, maxInky = 1, maxPinky = 1, maxClyde = 1;
    ArcadeText arcadeText;
    std::vector<std::vector<char>> map;
    std::string mapName, errorMessage, successMessage;
    const std::string folderPath = "../../Tappa_15/resources/maps/";

    GameMenu menu;
    std::vector<MenuOption> options;

    Create(sf::RenderWindow &window, MapEditor &me);
    Create(sf::RenderWindow &window, MapEditor &mapEditor, std::string mapName);

public:
    void doGraphics();
    void doUI();
    void setCursorPos(int x, int y);
    void handle(const sf::Event::MouseButtonPressed &mouseButton);
    void handle(const sf::Event::KeyPressed &key);
    void handle(const sf::Event::TextEntered &textEntered);
    void handle(const sf::Event::MouseMoved &mouseMoved);

private:
    void drawCursor();
    bool isCursorOnMap();
    void drawMap();
    void drawInputText();
    void saveMap();
    void loadMap();
    void drawTile();
    void deleteTile();
    void reset();
};