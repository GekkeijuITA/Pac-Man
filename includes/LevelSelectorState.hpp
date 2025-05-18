#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>

#include "StateManager.hpp"

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
    std::vector<MapPreview> maps;
    std::regex r;
    int page = 0, cols = 3, rows = 2;
    float margin = 20.f;
    sf::Texture straightLine;
    sf::Texture cornerTile;

public:
    LevelSelectorState(sf::RenderWindow &window);
    void draw();
    void loadMaps();

private:
    sf::Texture generateMapPreview(const std::string path);
    void drawCursor();
    std::string prettify(std::string name);
};