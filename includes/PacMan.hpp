#include <SFML/Graphics.hpp>
#include "global_values.hpp"
#include <vector>

struct PacMan
{
    float speed;
    std::vector<std::vector<char>> *map;
    sf::Texture tex;
    sf::Vector2u texSize;
    sf::Vector2f scale;
    sf::Vector2f origin;
    sf::Vector2i position;
    sf::Vector2f fPosition;

    enum Direction
    {
        UP = 1,
        DOWN = 3,
        LEFT = 0,
        RIGHT = 2,
        NONE = 4
    };

    Direction direction;
    Direction nextDirection;

    PacMan();
    void draw(sf::RenderWindow &window);
    void move(float elapsed);
    void setPosition(int x, int y);
    void setRotation(Direction dir);
    void setMap(std::vector<std::vector<char>> *map);
    bool isWall(int x, int y);
    void eat(int x, int y);
    void updateDirection();
};