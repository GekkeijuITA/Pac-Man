#include <SFML/Graphics.hpp>
#include "global_values.hpp"
#include <vector>
#include <map>

inline const sf::Vector2i RIGHT_PACMAN = {1,0};
inline const sf::Vector2i LEFT_PACMAN = {1,1};
inline const sf::Vector2i UP_PACMAN = {1,2};
inline const sf::Vector2i DOWN_PACMAN = {1,3};

struct PacMan
{
    float speed;
    std::vector<std::vector<char>> *map;
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

    const std::map<Direction, sf::Vector2i> PACMAN_MAP = {
        {RIGHT, RIGHT_PACMAN},
        {LEFT, LEFT_PACMAN},
        {UP, UP_PACMAN},
        {DOWN, DOWN_PACMAN},
        {NONE, RIGHT_PACMAN} // Default
    };

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