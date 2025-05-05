#pragma once

#include <SFML/Graphics.hpp>
#include "global_values.hpp"
#include <map>

struct Ghost
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
    Direction lastDirection;

    protected:
        std::map<Direction, sf::Vector2i> GHOST_TEX_MAP;

    Ghost();

    public:
        void draw(sf::RenderWindow &window);
        void setPosition(int x, int y);
        void setMap(std::vector<std::vector<char>> *map);
        void move(float elapsed);
        void setDirection(Direction dir);
        void chooseDirection();
        bool isWall(int x, int y);
};