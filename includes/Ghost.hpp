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
    std::vector<sf::Vector2i> exitTiles;
    sf::Vector2i nearestExitTile;

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

    enum GhostState {
        IN_HOUSE,
        NORMAL,
    };

    GhostState state;
    float timerToLeaveHouse;

    protected:
        std::map<Direction, sf::Vector2i> GHOST_TEX_MAP;

    Ghost(GhostState state, float timerToLeaveHouse);

    private:
        void setDirection(Direction dir);
        void chooseDirection();
        sf::Vector2i getNearestExitTile();

    public:
        void draw(sf::RenderWindow &window);
        void setPosition(int x, int y);
        void setMap(std::vector<std::vector<char>> *map);
        void move(float elapsed);
        bool isWall(int x, int y);
        void setState(GhostState state);
        void addExitTile(int x, int y);
};