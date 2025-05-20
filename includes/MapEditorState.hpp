#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "GameMenu.hpp"

struct MapEditorState
{
    sf::RenderWindow &window;
    GameMenu menu;
    std::vector<std::string> options = {"CREATE", "EDIT", "BACK"};
    ArcadeText arcadeText;
    int cursorIndex = 0;

    MapEditorState(sf::RenderWindow &window);

public:
    void doGraphics();
};