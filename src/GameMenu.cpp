#include "../includes/GameMenu.hpp"

#include <iostream>

GameMenu::GameMenu() {}

GameMenu::GameMenu(sf::View v, std::string title, std::vector<std::string> options) : view(v), title(title), options(options)
{
    if (!tex.loadFromFile(TEXT))
    {
        std::cerr << "Error loading texture" << std::endl;
        exit(1);
    }

    ArcadeText arcadeText;
}

void GameMenu::draw(sf::RenderWindow &window)
{
    sf::RectangleShape background(view.getSize());
    background.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(background);

    arcadeText.drawString(title, 10, 3, window, 2.f);
    for (auto &option : options)
    {
        arcadeText.drawString(option, 11, FIRST_OPTION + (2 * (&option - &options[0])), window);
    }

    sf::CircleShape triangle(TILE_SIZE / 2.f, 3);
    triangle.setFillColor(sf::Color::Red);
    triangle.setPosition({TILE_SIZE * (cursorPosition.x + .5f), TILE_SIZE * (cursorPosition.y + .5f)});
    triangle.setOrigin({TILE_SIZE / 2.f, TILE_SIZE / 2.f});
    triangle.setRotation(sf::degrees(90));
    window.draw(triangle);
}

void GameMenu::setCursorPosition(int x, int y)
{
    cursorPosition = {x, y};
}

sf::Vector2i GameMenu::getCursorPosition()
{
    return cursorPosition;
}