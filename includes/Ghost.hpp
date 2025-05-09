#pragma once

#include <SFML/Graphics.hpp>
#include "global_values.hpp"
#include <map>
#include <string>

inline const sf::Vector2i GHOST_SCARED = {8, 4};
inline const sf::Vector2i GHOST_R = {8, 5};
inline const sf::Vector2i GHOST_L = {9, 5};
inline const sf::Vector2i GHOST_U = {10, 5};
inline const sf::Vector2i GHOST_D = {11, 5};

struct State;

struct Ghost
{
    float speed;
    int dotLimit;
    bool isTransitioning, isGoingToExit, isGoingToSpawn;
    std::vector<std::vector<char>> *map;
    sf::Texture tex;
    sf::Vector2i position;
    sf::Vector2f fPosition;
    sf::Vector2i spawn;
    std::vector<sf::Vector2i> exitTiles;
    sf::Vector2i nearestExitTile;
    // sf::IntRect preferredZone;
    std::string name;
    State &gameState;

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

    enum GhostState
    {
        IN_HOUSE,
        NORMAL,
        EATEN,
        SCARED,
        RETURNING_FROM_SCARED,
    };

    GhostState state;

protected:
    std::map<Direction, sf::Vector2i> GHOST_TEX_MAP;
    std::map<Direction, sf::Vector2i> GHOST_EYES_TEX_MAP = {
        {UP, GHOST_U},
        {DOWN, GHOST_D},
        {LEFT, GHOST_L},
        {RIGHT, GHOST_R},
        {NONE, GHOST_R}};

    Ghost(
        GhostState state,
        int dotLimit,
        /*sf::IntRect preferredZone,*/
        std::string name,
        State &gameState);

private:
    void setDirection(Direction dir);
    void chooseDirection();
    sf::Vector2i getNearestExitTile();
    void eat(int x, int y);
    void followPathTo(sf::Vector2i destination, bool &isGoing);
    std::vector<sf::Vector2i> findPathBFS(sf::Vector2i destination);

public:
    void draw(sf::RenderWindow &window);
    void setPosition(int x, int y);
    void setMap(std::vector<std::vector<char>> *map);
    void move(float elapsed);
    bool isWall(int x, int y);
    // bool isInsideZone(int x, int y);
    void setState(GhostState state);
    void addExitTile(int x, int y);
    void respawn(GhostState state);
};