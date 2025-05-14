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

inline const sf::Vector2i GHOST_SCORE_200 = {0, 8};
inline const sf::Vector2i GHOST_SCORE_400 = {1, 8};
inline const sf::Vector2i GHOST_SCORE_800 = {2, 8};
inline const sf::Vector2i GHOST_SCORE_1600 = {3, 8};

struct GameState;

struct Ghost
{
    float speed, currentSpeed, timeToEnterHouse, scoreDisplayTimer;
    int dotLimit, score;
    bool isTransitioning, enteredHouse, stoppedForScore;
    std::vector<std::vector<char>> *map;
    sf::Texture tex;
    sf::Vector2i position;
    sf::Vector2f fPosition;
    sf::Vector2i spawn;
    std::vector<sf::Vector2i> exitTiles;
    sf::Vector2i nearestExitTile;
    std::string name;
    GameState &gameState;
    std::vector<sf::Vector2i> path;

    Direction direction;
    Direction lastDirection;

    enum GhostState
    {
        IN_HOUSE,
        NORMAL,
        EATEN,
        SCARED,
    };

    GhostState state, lastState;

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
        std::string name,
        GameState &gameState);

private:
    void setDirection(Direction dir);
    void chooseDirection();
    void getExitTile();
    void eat(int x, int y);
    void findPathBFS(sf::Vector2i destination);
    void computeNextDirection(sf::Vector2i destination);
    Direction getOppositeDirection(Direction dir);
    sf::Vector2i getPacmanPosition();

public:
    void draw(sf::RenderWindow &window);
    void setPosition(int x, int y);
    void setMap(std::vector<std::vector<char>> *map);
    void move(float elapsed);
    double distance(sf::Vector2i target);
    bool isWall(int x, int y);
    void setState(GhostState state);
    void addExitTile(int x, int y);
    void respawn(GhostState state);
    void drawScore();
};