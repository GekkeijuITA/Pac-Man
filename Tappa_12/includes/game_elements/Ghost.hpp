#pragma once

#include <SFML/Graphics.hpp>
#include "../lib/global_values.hpp"
#include "../core/Animation.hpp"
#include <map>
#include <string>
#include <memory>

#define COLLIDE_BOX 1.1f
#define SCORE_DISPLAY_TIME 1.5f;
#define GHOST_SPEED 2.5f

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
    float speed, currentSpeed = 0.f, timeToEnterHouse = 0.f, blinkingTime = 0.2f;
    float scoreDisplayTimer = SCORE_DISPLAY_TIME;
    int dotLimit, score = 0;
    const double frameDuration = 0.2;
    bool isTransitioning = false, enteredHouse = false, stoppedForScore = false, isWhite = true;

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

    Direction direction = NONE;
    Direction lastDirection = NONE;

    enum GhostState
    {
        IN_HOUSE,
        NORMAL,
        EATEN,
        SCARED,
    };

    GhostState state = IN_HOUSE, lastState;

    std::unique_ptr<sf::Sprite> sprite;
    std::map<Direction, Animation> GHOST_ANIM_MAP;
    std::unique_ptr<Animation> scaredAnim;
    std::unique_ptr<Animation> backScaredAnim;

protected:
    std::map<Direction, sf::Vector2i> GHOST_TEX_MAP;
    std::map<Direction, sf::Vector2i> GHOST_EYES_TEX_MAP = {
        {UP, GHOST_U},
        {DOWN, GHOST_D},
        {LEFT, GHOST_L},
        {RIGHT, GHOST_R},
        {NONE, GHOST_L}};

    Ghost(
        GhostState state,
        int dotLimit,
        std::string name,
        GameState &gameState,
        std::map<Direction, sf::Vector2i> GHOST_TEX_MAP);

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
    void setSpeed();
};