#include <SFML/Graphics.hpp>

struct PacMan
{
    float speed;
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

    PacMan();
    void draw(sf::RenderWindow &window);
    void move(float elapsed);
    void setPosition(int x, int y);
    void setRotation(Direction dir);
};