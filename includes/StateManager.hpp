#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.hpp"
#include "MainMenuState.hpp"
#include "LevelSelectorState.hpp"
#include <memory>

struct GameState;
struct MainMenuState;
struct LevelSelectorState;

struct StateManager
{
    enum Mode
    {
        MAIN_MENU,
        NORMAL_GAME,
        LEVEL_SELECTOR
    };

    Mode currentMode = MAIN_MENU;
    std::unique_ptr<GameState> gameState;
    std::unique_ptr<MainMenuState> mainMenuState;
    std::unique_ptr<LevelSelectorState> levelSelectorState;
    
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