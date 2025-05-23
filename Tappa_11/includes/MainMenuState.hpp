#pragma once

#include <SFML/Graphics.hpp>
#include "GameMenu.hpp"
#include "StateManager.hpp"

struct StateManager;

struct MainMenuState
{
    sf::RenderWindow &window;
    GameMenu menu;
    StateManager &stateManager;
    ArcadeText arcadeText;
    int highscore;
    sf::Vector2i cursorPosition = {10, 9};
    int cursorIndex = 0;
    std::vector<std::string> options = {"PLAY", "MAP EDITOR" , "QUIT"};

    MainMenuState(sf::RenderWindow &window, StateManager &stateManager);

public:
    void draw();
    void getHighscore();

private:
    void drawScore(int x, int y, int score);
};