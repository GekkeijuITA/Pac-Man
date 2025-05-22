#pragma once

#include <SFML/Graphics.hpp>
#include "GameMenu.hpp"
#include "core/StateManager.hpp"

struct StateManager;

struct MainMenuState
{
    sf::RenderWindow &window;
    GameMenu menu;
    StateManager &stateManager;
    ArcadeText arcadeText;
    int highscore;
    std::vector<MenuOption> options;

    MainMenuState(sf::RenderWindow &window, StateManager &stateManager);

public:
    void draw();
    void getHighscore();

private:
    void drawScore(int x, int y, int score);
};