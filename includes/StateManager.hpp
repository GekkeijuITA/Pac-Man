#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.hpp"
#include <memory>

struct StateManager
{
    enum Mode
    {
        MAIN_MENU,
        NORMAL_GAME
    };

    Mode currentMode = MAIN_MENU;
    std::unique_ptr<GameState> gameState;
    sf::RenderWindow window;
    unsigned w, h;
    std::string title;

    StateManager(unsigned w, unsigned h, std::string title);

public:
    void update(float elapsed);
    void doGraphics();
    void handleInputs();
    void initGame(std::string mapPath);
};