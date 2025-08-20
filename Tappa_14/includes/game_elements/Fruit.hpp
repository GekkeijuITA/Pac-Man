#pragma once

#include <SFML/Graphics.hpp>
#include "../lib/global_values.hpp"
#include "../lib/textures.hpp"

#include <iostream>
#include <time.h>
#include <memory>

inline const sf::Vector2i FRUIT_SCORE_100 = {0, 9};
inline const sf::Vector2i FRUIT_SCORE_300 = {1, 9};
inline const sf::Vector2i FRUIT_SCORE_500 = {2, 9};
inline const sf::Vector2i FRUIT_SCORE_700 = {3, 9};
inline const sf::Vector2i FRUIT_SCORE_1000 = {4, 9};
inline const sf::Vector2i FRUIT_SCORE_2000 = {4, 10};
inline const sf::Vector2i FRUIT_SCORE_3000 = {4, 11};
inline const sf::Vector2i FRUIT_SCORE_5000 = {4, 12};

#define SCORE_DISPLAY_TIME 1.5f;
#define FRUIT_BLINKING_TIME 0.3f;

struct GameState;

struct Fruit
{
    sf::Texture tex;
    sf::Vector2i position;
    int score;
    float fruitDisplayTimer, scoreDisplayTimer, blinkingTime = FRUIT_BLINKING_TIME;
    bool isBlinking = false;
    bool isEaten = false;
    bool isVisible = false;
    GameState &gameState;

protected:
    Fruit(
        sf::Vector2i position,
        sf::Vector2i texPosition,
        int score,
        GameState &gameState);

public:
    void draw(sf::RenderWindow &window, char tileType);
    int getScore();
    void setTimer();
    void drawScore(sf::RenderWindow &window);
    void update(float elapsed);
};