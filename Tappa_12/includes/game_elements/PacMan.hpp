#pragma once

#include <SFML/Graphics.hpp>
#include "../lib/global_values.hpp"
#include "../core/Animation.hpp"
#include <vector>
#include <map>
#include <memory>

inline const sf::Vector2i RIGHT_PACMAN = {1, 0};
inline const sf::Vector2i LEFT_PACMAN = {1, 1};
inline const sf::Vector2i UP_PACMAN = {1, 2};
inline const sf::Vector2i DOWN_PACMAN = {1, 3};
inline const sf::Vector2i PACMAN_FULL = {2, 0};

inline const sf::Vector2i DEATH_1 = {3, 0};
inline const sf::Vector2i DEATH_2 = {4, 0};
inline const sf::Vector2i DEATH_3 = {5, 0};
inline const sf::Vector2i DEATH_4 = {6, 0};
inline const sf::Vector2i DEATH_5 = {7, 0};
inline const sf::Vector2i DEATH_6 = {8, 0};
inline const sf::Vector2i DEATH_7 = {9, 0};
inline const sf::Vector2i DEATH_8 = {10, 0};
inline const sf::Vector2i DEATH_9 = {11, 0};
inline const sf::Vector2i DEATH_10 = {12, 0};
inline const sf::Vector2i DEATH_11 = {13, 0};

inline const float POWER_PELLET_DURATION = 5.f;

struct GameState;

struct PacMan
{
    std::unique_ptr<sf::Sprite> sprite;

    float speed, powerPelletDuration;
    int dotEaten, ghostStreak;
    bool powerPellet, isDead = false;

    std::vector<std::vector<char>> *map;
    sf::Texture tex;
    sf::Vector2i position;
    sf::Vector2f fPosition;
    sf::Vector2i spawn;
    GameState &gameState;

    Direction direction;
    Direction nextDirection;

    const std::map<Direction, sf::Vector2i> PACMAN_TEX_MAP = {
        {RIGHT, RIGHT_PACMAN},
        {LEFT, LEFT_PACMAN},
        {UP, UP_PACMAN},
        {DOWN, DOWN_PACMAN},
        {NONE, RIGHT_PACMAN} // Default
    };

    std::map<Direction, Animation> PACMAN_ANIM_MAP;

    std::vector<sf::Vector2i> deathFrames = {DEATH_1, DEATH_2, DEATH_3, DEATH_4, DEATH_5, DEATH_6, DEATH_7, DEATH_8, DEATH_9, DEATH_10, DEATH_11};
    std::vector<Animation> DEATH_ANIMATION;

    PacMan(GameState &gameState);
    void draw(sf::RenderWindow &window);
    void move(float elapsed);
    void setPosition(int x, int y);
    void setNextDirection(Direction dir);
    void setMap(std::vector<std::vector<char>> *map);
    bool isWall(int x, int y);
    void eat(int x, int y);
    void updateDirection();
    int getDotEaten();
    void respawn();
    void setDeathAnimation();
    sf::Vector2i getPosition();
};