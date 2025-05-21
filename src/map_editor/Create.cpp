#include "../../includes/map_editor/Create.hpp"
#include "../../includes/core/Debug.hpp"
#include "../../includes/lib/global_values.hpp"
#include "../../includes/lib/textures.hpp"

#include <iostream>

Create::Create(sf::RenderWindow &window) : window(window)
{
    if (!straightLine.loadFromFile(STRAIGHT_LINE_H))
    {
        std::cerr << "Error loading straight line texture" << std::endl;
    }

    if (!angle.loadFromFile(ANGLE_0))
    {
        std::cerr << "Error loading angle texture" << std::endl;
    }

    if (!pacDot.loadFromFile(PACDOT_TEX))
    {
        std::cerr << "Error loading pacdot texture" << std::endl;
    }

    cursorPos = {0, 0};
}

void Create::doGraphics()
{
    Debug::drawGrid(window);
    drawCursor();
    doUI();
}

void Create::drawCursor()
{
    if (cursorPos.x < 0 || cursorPos.x >= MAP_WIDTH || cursorPos.y < 0 || cursorPos.y >= MAP_HEIGHT)
    {
        return;
    }

    sf::RectangleShape cursor(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    cursor.setPosition({static_cast<float>(cursorPos.x) * TILE_SIZE, static_cast<float>(cursorPos.y) * TILE_SIZE});
    cursor.setFillColor(sf::Color::Transparent);
    cursor.setOutlineThickness(1.f);
    cursor.setOutlineColor(sf::Color::Red);
    window.draw(cursor);
}

void Create::setCursorPos(int x, int y)
{
    cursorPos.x = x;
    cursorPos.y = y;
}

void Create::doUI()
{
    sf::RectangleShape outline(sf::Vector2f(TILE_SIZE * MAP_WIDTH, TILE_SIZE * 5));
    outline.setPosition({0.f, TILE_SIZE * MAP_HEIGHT});
    outline.setFillColor(sf::Color::Black);
    outline.setOutlineThickness(1.f);
    outline.setOutlineColor(Debug::gridColor);
    window.draw(outline);

    float x = 0.f;
    float y = MAP_HEIGHT * TILE_SIZE;

    sf::Sprite pacdot(pacDot);
    pacdot.setScale({(float)TILE_SIZE / pacDot.getSize().x, (float)TILE_SIZE / pacDot.getSize().y});
    pacdot.setPosition({x, y});
    window.draw(pacdot);

    sf::Sprite wall(straightLine);
    wall.setScale({(float)TILE_SIZE / straightLine.getSize().x, (float)TILE_SIZE / straightLine.getSize().y});
    wall.setPosition({x + TILE_SIZE, y});
    window.draw(wall);

    sf::Sprite corner(angle);
    corner.setScale({(float)TILE_SIZE / angle.getSize().x, (float)TILE_SIZE / angle.getSize().y});
    corner.setPosition({x + (TILE_SIZE * 2), y});
    window.draw(corner);

    float scale = 3.f;
    sf::CircleShape powerpellet(TILE_SIZE / scale);
    powerpellet.setOrigin({TILE_SIZE / scale, TILE_SIZE / scale});
    powerpellet.setPosition({(x + (TILE_SIZE * 3)) + (TILE_SIZE / 2), (y) + (TILE_SIZE / 2)});
    powerpellet.setFillColor(sf::Color(255, 185, 176));
    window.draw(powerpellet);

    sf::RectangleShape ghostDoor({TILE_SIZE, TILE_SIZE / 4});
    ghostDoor.setPosition({x + (TILE_SIZE * 4), (y) + TILE_SIZE / 1.8f});
    ghostDoor.setFillColor(sf::Color(255, 203, 255));
    window.draw(ghostDoor);
}