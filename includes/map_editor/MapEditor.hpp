#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "../GameMenu.hpp"
#include "../core/StateManager.hpp"

struct StateManager;

struct MapEditor
{
    enum Mode {
        MENU,
        CREATE,
        EDIT
    };

    Mode currentMode = MENU;

    sf::RenderWindow &window;
    GameMenu menu;
    StateManager &stateManager;

    std::vector<MenuOption> options;
    ArcadeText arcadeText;
    int cursorIndex = 0;

    MapEditor(sf::RenderWindow &window, StateManager &sm);

public:
    void doGraphics();
};