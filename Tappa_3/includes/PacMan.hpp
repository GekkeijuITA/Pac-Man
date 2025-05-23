#include <SFML/Graphics.hpp>
#include "global_values.hpp"

struct PacMan
{
    float speed;
    char (*map)[MAP_HEIGHT];
    sf::Texture tex;
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
    void setMap(char (*newMap)[MAP_HEIGHT]);
    bool isWall(int x, int y);
    void eat(int x, int y);
    void updateDirection();
};