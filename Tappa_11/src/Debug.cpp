#include "../includes/Debug.hpp"

void Debug::drawGrid(sf::RenderWindow &window)
{
    sf::Color gridColor = sf::Color(255, 255, 255, 100);
    float thickness = 1.0f;

    for (int x = 0; x <= MAP_WIDTH; x++)
    {
        sf::RectangleShape line(sf::Vector2f({thickness, (MAP_HEIGHT + 5) * TILE_SIZE}));
        line.setPosition({(float)x * TILE_SIZE, 0});
        line.setFillColor(gridColor);
        window.draw(line);
    }

    for (int y = 0; y <= MAP_HEIGHT + 5; y++)
    {
        sf::RectangleShape line(sf::Vector2f({MAP_WIDTH * TILE_SIZE, thickness}));
        line.setPosition({0, (float)y * TILE_SIZE});
        line.setFillColor(gridColor);
        window.draw(line);
    }
}