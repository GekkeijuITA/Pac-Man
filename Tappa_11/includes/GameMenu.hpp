#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "../includes/text_ui.hpp"
#include "../includes/textures.hpp"
#include "../includes/global_values.hpp"

struct GameMenu
{
    sf::Texture tex;
    sf::View view;
    sf::Vector2i cursorPosition;
    ArcadeText arcadeText;
    std::string title;
    std::vector<std::string> options;
    sf::Vector2i titlePos;
    int cursorIndex = 0;

    GameMenu();
    GameMenu(sf::View v, std::string title, std::vector<std::string> options, sf::Vector2i titlePos);

public:
    void draw(sf::RenderWindow &window);
    void setCursorPosition(int x, int y);
    sf::Vector2i getCursorPosition();
    int getOptionsSize();
};