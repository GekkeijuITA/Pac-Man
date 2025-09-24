#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "../ui/GameMenu.hpp"
#include "../core/StateManager.hpp"
#include "Create.hpp"
#include "../LevelSelectorState.hpp"

struct StateManager;
struct LevelSelectorState;

struct Create;

struct MapEditor
{
    enum Mode {
        MENU,
        CREATE,
        EDIT
    };

    Mode currentMode = MENU;

    sf::RenderWindow &window;
    
    std::vector<MenuOption> options = {};
    GameMenu menu;

    StateManager &stateManager;
    ArcadeText arcadeText;
    int cursorIndex = 0;

    std::unique_ptr<Create> create;
    std::unique_ptr<LevelSelectorState> levelSelectorState;

    MapEditor(sf::RenderWindow &window, StateManager &sm);

public:
    void doGraphics();
    void initEditor(std::string mapName);
    void handle(const sf::Event::MouseButtonPressed &mouse);
    void handle(const sf::Event::MouseMoved &mouse);
    void handle(const sf::Event::MouseWheelScrolled &wheel);
};