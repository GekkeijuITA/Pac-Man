#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>

#include "core/StateManager.hpp"
#include "map_editor/MapEditor.hpp"
#include "scenario_editor/ScenarioEditor.hpp"
#include "game_elements/GameState.hpp"

struct StateManager;
struct MapEditor;
struct ScenarioEditor;
struct GameState;

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
    StateManager &stateManager;
    MapEditor &mapEditor;
    ScenarioEditor &scenarioEditor;
    GameState &gameState;
    ArcadeText arcadeText;
    sf::Vector2i cursorPosition = {0, 0};
    std::regex r;
    int page = 0, maxCols = 3, maxRows = 3, totalPages = 1;
    float margin = 20.f, TILE_SIZE_PREVIEW;
    sf::Texture straightLine;
    sf::Texture cornerTile;
    sf::Texture pacDot;
    std::vector<std::vector<MapPreview>> maps;
    std::string title;

public:
    LevelSelectorState(sf::RenderWindow &window, StateManager &sm, MapEditor &me, std::string title = "Map Selector");
    LevelSelectorState(sf::RenderWindow &window, StateManager &sm, ScenarioEditor &se, std::string title = "Map Selector");
    LevelSelectorState(sf::RenderWindow &window, StateManager &sm, GameState &gs, std::string title = "Map Selector");
    void draw();
    void loadMaps();
    void moveCursorLeft();
    void moveCursorRight();
    void moveCursorUp();
    void moveCursorDown();
    void handle(const sf::Event::KeyPressed &key);
    void handle(const sf::Event::MouseButtonPressed &button);
    void handle(const sf::Event::MouseMoved &mouse);
    void handle(const sf::Event::MouseWheelScrolled &wheel);

private:
    sf::Texture generateMapPreview(const std::string path);
    void drawCursor();
    void drawSelectedCursor();
    std::string getMapName(std::string path);
    std::string prettify(std::string name);
    void nextPage();
    void previousPage();
    std::string getScenarioName();
};