#pragma once

#include <SFML/Graphics.hpp>
#include "global_values.hpp"
#include "Animation.hpp"
#include <vector>
#include <map>
#include <memory>

inline const sf::Vector2i RIGHT_PACMAN = {1, 0};
inline const sf::Vector2i LEFT_PACMAN = {1, 1};
inline const sf::Vector2i UP_PACMAN = {1, 2};
inline const sf::Vector2i DOWN_PACMAN = {1, 3};
inline const sf::Vector2i PACMAN_FULL = {2, 0};
inline const float POWER_PELLET_DURATION = 5.f;

struct GameState;

struct PacMan
{
    std::unique_ptr<sf::Sprite> sprite;

    float speed, powerPelletDuration;
    int dotEaten, ghostStreak;
    bool powerPellet;

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

    PacMan(GameState &gameState);
    void draw(sf::RenderWindow &window);
    void move(float elapsed);
    void setPosition(int x, int y);
    void setRotation(Direction dir);
    void setMap(std::vector<std::vector<char>> *map);
    bool isWall(int x, int y);
    void eat(int x, int y);
    void updateDirection();
    int getDotEaten();
    void respawn();
    sf::Vector2i getPosition();
};