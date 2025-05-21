#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>

#include "core/StateManager.hpp"

struct MapPreview
{
    std::string name;
    std::string path;
    sf::Texture texture;
    sf::Vector2f position;
};

struct LevelSelectorState
{
    sf::RenderWindow &window;
    ArcadeText arcadeText;
    sf::Vector2i cursorPosition = {0, 0};
    std::regex r;
    int page = 0, maxCols = 3, maxRows = 3, totalPages = 1;
    float margin = 20.f, TILE_SIZE_PREVIEW;
    sf::Texture straightLine;
    sf::Texture cornerTile;
    std::vector<std::vector<MapPreview>> maps;

public:
    LevelSelectorState(sf::RenderWindow &window);
    void draw();
    void loadMaps();
    void moveCursorLeft();
    void moveCursorRight();
    void moveCursorUp();
    void moveCursorDown();

private:
    sf::Texture generateMapPreview(const std::string path);
    void drawCursor();
    std::string prettify(std::string name);
    void nextPage();
    void previousPage();
};