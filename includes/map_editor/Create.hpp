#pragma once
#include <SFML/Graphics.hpp>
#include <map>

struct Create
{
    sf::RenderWindow &window;
    sf::Vector2i cursorPos;
    sf::Texture straightLine, angle, pacDot;
    std::map<char, sf::Sprite> tileSprites;

    Create(sf::RenderWindow &window);

public:
    void doGraphics();
    void doUI();
    void setCursorPos(int x, int y);

private:
    void drawCursor();
};