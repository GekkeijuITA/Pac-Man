#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <functional>

#include "lib/text_ui.hpp"
#include "lib/textures.hpp"
#include "lib/global_values.hpp"

struct MenuOption
{
    std::string name;
    std::function<void()> action;
};

struct GameMenu
{
    sf::Texture tex;
    sf::View view;
    ArcadeText arcadeText;
    std::string title;
    std::vector<MenuOption> options;
    sf::Vector2i titlePos, startOptionsPos, cursorPosition;
    int cursorIndex = 0;
    TextColor textColor;
    float scaleFactor = 2.f;

    GameMenu();
    GameMenu(sf::View v, std::string title, sf::Vector2i titlePos, TextColor textColor, std::vector<MenuOption> options, sf::Vector2i startOptionsPos);
    GameMenu(sf::View v, std::string title, sf::Vector2i titlePos, TextColor textColor, float scaleFactor, std::vector<MenuOption> options, sf::Vector2i startOptionsPos);

public:
    void draw(sf::RenderWindow &window);
    void setCursorPosition(int x, int y);
    sf::Vector2i getCursorPosition();
    int getOptionsSize();
    void moveCursorUp();
    void moveCursorDown();
    void executeOption();
    void resetCursor();
    void handle(const sf::Event::KeyPressed &key);
};