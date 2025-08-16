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

inline const float ALIGNMENT_TOLERANCE = 0.6f;

struct GameState;

struct Ghost
{
    float speed, currentSpeed = 0.f, timeToEnterHouse = 0.f, blinkingTime = 0.2f;
    float scoreDisplayTimer = SCORE_DISPLAY_TIME;
    int dotLimit, score = 0, chaseScatterIndex = 0;
    const double frameDuration = 0.2;
    bool isTransitioning = false, enteredHouse = false, stoppedForScore = false, isWhite = true;
    bool isChasing = false;
    float chaseScatterPattern[7];

    std::vector<std::vector<char>> *map;
    sf::Texture tex;
    sf::Vector2i position;
    sf::Vector2f fPosition;
    sf::Vector2i spawn;
    sf::Vector2i lastDirectionChoicePosition = {-1, -1};
    std::vector<sf::Vector2i> exitTiles;
    sf::Vector2i nearestExitTile;
    std::string name;
    GameState &gameState;
    std::vector<sf::Vector2i> path;
    sf::IntRect preferredAngle;

    Direction direction = NONE;
    Direction lastDirection = NONE;

    // DEBUG
    sf::Vector2i targetTile;

    enum GhostState
    {
        IN_HOUSE,
        EATEN,
        SCARED,
        CHASE,
        SCATTER
    };

    GhostState state = IN_HOUSE, lastState;

    std::unique_ptr<sf::Sprite> sprite;
    std::map<Direction, Animation> GHOST_ANIM_MAP;
    std::unique_ptr<Animation> scaredAnim;
    std::unique_ptr<Animation> backScaredAnim;

protected:
    sf::Vector2i getPacmanPosition();
    Direction getPacmanDirection();
    void findPathBFS(sf::Vector2i destination);
    void setDirection(Direction dir);
    void computeNextDirection(sf::Vector2i destination);
    void exitHouse();
    virtual void behaviour();
    bool isAlignedToCell(float tolerance = ALIGNMENT_TOLERANCE);
    sf::Vector2i findFirstValidTile(sf::Vector2i start, int dimensions);
    bool isWall(int x, int y);
    void chooseDirection();

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
        sf::IntRect preferredAngle,
        std::string name,
        GameState &gameState,
        std::map<Direction, sf::Vector2i> GHOST_TEX_MAP);

private:
    void getExitTile();
    Direction getOppositeDirection(Direction dir);
    void move(float elapsed);
    void drawScore();
    double distance(sf::Vector2i target);
    void eat(int x, int y);
    void eatenState(float elapsed);
    void scatterState();
    void animate(float elapsed);
    bool isAtIntersection();

public:
    void draw(sf::RenderWindow &window);
    void setPosition(int x, int y);
    void setMap(std::vector<std::vector<char>> *map);
    void update(float elapsed);
    void setState(GhostState state);
    void addExitTile(int x, int y);
    virtual void respawn();
    void setSpeed();
    void setScatterChasePattern();
};