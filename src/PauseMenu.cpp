#include "../includes/PauseMenu.hpp"

#include <iostream>

PauseMenu::PauseMenu()
{
    if (!tex.loadFromFile(TEXT))
    {
        std::cerr << "Error loading texture" << std::endl;
        exit(1);
    }
}

PauseMenu::PauseMenu(sf::View v) : view(v)
{
    if (!tex.loadFromFile(TEXT))
    {
        std::cerr << "Error loading texture" << std::endl;
        exit(1);
    }
}

void PauseMenu::draw(sf::RenderWindow &window)
{
    sf::RectangleShape background(view.getSize());
    background.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(background);

    drawString("PAUSE", 10, 3, window, 2.f);
    drawString("CONTINUE", 11, FIRST_OPTION, window);
    drawString("RESTART", 11, FIRST_OPTION + 2, window);
    drawString("QUIT", 11, LAST_OPTION, window);

    sf::CircleShape triangle(TILE_SIZE / 2.f, 3);
    triangle.setFillColor(sf::Color::Red);
    triangle.setPosition({TILE_SIZE * (cursorPosition.x + .5f), TILE_SIZE * (cursorPosition.y + .5f)});
    triangle.setOrigin({TILE_SIZE / 2.f, TILE_SIZE / 2.f});
    triangle.setRotation(sf::degrees(90));
    window.draw(triangle);
}

void PauseMenu::drawChar(int x, int y, sf::Vector2i charPos, sf::RenderWindow &window, float scaleFactor, int i)
{
    sf::Sprite textSprite = createSprite(
        tex,
        charPos,
        {(float)TILE_SIZE / TEXT_SIZE, (float)TILE_SIZE / TEXT_SIZE},
        scaleFactor,
        TEXT_SIZE,
        false);

    textSprite.setPosition({(x * TILE_SIZE) + ((scaleFactor * i) * TILE_SIZE), (float)y * TILE_SIZE});
    window.draw(textSprite);
}

void PauseMenu::drawString(std::string str, int x, int y, sf::RenderWindow &window, float scaleFactor)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        std::string c(1, str[i]);
        drawChar(x, y, CHAR_MAP.at(c), window, scaleFactor, i);
    }
}

void PauseMenu::setCursorPosition(int x, int y)
{
    cursorPosition = {x, y};
}

sf::Vector2i PauseMenu::getCursorPosition()
{
    return cursorPosition;
}