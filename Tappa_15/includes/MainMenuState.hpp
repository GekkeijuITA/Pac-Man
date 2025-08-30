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

    std::vector<MenuOption> options = {};
    GameMenu menu;
    
    StateManager &stateManager;
    ArcadeText arcadeText;
    int highscore;

    MainMenuState(sf::RenderWindow &window, StateManager &stateManager);

public:
    void draw();
    void getHighscore();
    void handle(const sf::Event::MouseButtonPressed &mouse);
    void handle(const sf::Event::MouseMoved &mouse);

private:
    void drawScore(int x, int y, int score);
};