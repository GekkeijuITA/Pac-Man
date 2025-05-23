#include <SFML/Graphics.hpp>
#include "../lib/global_values.hpp"

struct Debug
{
    static inline sf::Color gridColor {255, 255, 255, 100};
    static void drawGrid(sf::RenderWindow &window);
};