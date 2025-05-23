#pragma once

#include <SFML/Graphics.hpp>
#include "GameMenu.hpp"
#include "core/StateManager.hpp"
#include "map_editor/MapEditor.hpp"

struct StateManager;
struct MapEditor;

struct MainMenuState
{
    sf::RenderWindow &window;
    GameMenu menu;
    StateManager &stateManager;
    MapEditor &mapEditor;
    ArcadeText arcadeText;
    int highscore;
    std::vector<MenuOption> options;

    MainMenuState(sf::RenderWindow &window, StateManager &stateManager, MapEditor &mapEditor);

public:
    void draw();
    void getHighscore();

private:
    void drawScore(int x, int y, int score);
};