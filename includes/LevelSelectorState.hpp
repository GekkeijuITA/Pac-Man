#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <string>
#include <regex>
#include <fstream>

#include "StateManager.hpp"

struct MapPreview
{
    std::string name;
    std::string path;
    sf::Sprite sprite;
    sf::Vector2f position;
};

struct LevelSelectorState
{
    sf::RenderWindow &window;
    ArcadeText arcadeText;
    sf::Vector2i cursorPosition = {0, 0};
    std::vector<MapPreview> maps;
    std::regex r;
    int page = 0, cols = 2, rows = 2;
    float spacing = 64.f;
    sf::Texture straightLine;
    sf::Texture cornerTile;

public:
    LevelSelectorState(sf::RenderWindow &window);
    void draw();
    void loadMaps();

private:
    sf::Sprite generateMapPreview(const std::string path);
    void drawMapPreview(const MapPreview &map);
    void drawCursor();
    std::string prettify(std::string name);
};