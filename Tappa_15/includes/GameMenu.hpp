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
    sf::RenderWindow &window;

    sf::Vector2i titlePos, startOptionsPos, cursorPosition;
    int cursorIndex = 0;
    TextColor textColor;
    float scaleFactor = 2.f;
    float optionsBoxHeight, optionsBoxWidth;
    sf::FloatRect optionsBox;

    GameMenu(std::string title, sf::Vector2i titlePos, TextColor textColor, std::vector<MenuOption> options, sf::Vector2i startOptionsPos, sf::RenderWindow &window);
    GameMenu(std::string title, sf::Vector2i titlePos, TextColor textColor, float scaleFactor, std::vector<MenuOption> options, sf::Vector2i startOptionsPos, sf::RenderWindow &window);

public:
    void draw();
    void setCursorPosition(int x, int y);
    sf::Vector2i getCursorPosition();
    int getOptionsSize();
    void moveCursorUp();
    void moveCursorDown();
    void executeOption();
    void resetCursor();
    void setOptions(const std::vector<MenuOption> &options);
    void handle(const sf::Event::KeyPressed &key);
    void handle(const sf::Event::MouseButtonPressed &mouse);
    void handle(const sf::Event::MouseMoved &mouse);
};