#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <cmath>
#include <vector>
#include <deque>
#include <memory>

#include "PacMan.hpp"
#include "ghosts/Blinky.hpp"
#include "ghosts/Pinky.hpp"
#include "ghosts/Inky.hpp"
#include "ghosts/Clyde.hpp"

#include "fruits/Cherry.hpp"
#include "fruits/Orange.hpp"
#include "fruits/Strawberry.hpp"
#include "fruits/Grape.hpp"
#include "fruits/Apple.hpp"
#include "fruits/Bell.hpp"
#include "fruits/Key.hpp"
#include "fruits/Galaxian.hpp"

#include "../GameMenu.hpp"
#include "../core/StateManager.hpp"

#define GAME_OVER_TIME .5f
#define START_GAME_TIME 3.f
#define LIVES 3
#define VICTORY_LEVEL 2
#define WALL_BLINK_TIME 0.3f
#define VICTORY_TIME 3.f

struct StateManager;

struct TextureData
{
    sf::Texture texture;
    sf::Vector2f scale;
};

struct FruitData
{
    sf::Vector2i position;
    std::unique_ptr<Fruit> fruit;
    bool isEaten = false;
    bool isVisible = false;
};

struct GameState
{
    sf::RenderWindow &window;
    bool gameOver = false, pause = false, startGame = true, victory = false, isWallBlinking = false, isWallWhite = false;

    PacMan pacman;
    std::vector<std::vector<char>> map;
    std::string mapPath;
    std::vector<Ghost *> ghosts;
    sf::Texture wallBlinkTexture, cornerBlinkTexture;

    Blinky blinky;
    Pinky pinky;
    Inky inky;
    Clyde clyde;

    int lives = LIVES, score = 0, highscore = 0, level = 1, eatableTiles = 0;

    size_t maxFruits = 12 - LIVES;
    std::deque<char> eatenFruits;
    std::multimap<char, FruitData> fruits;

    GameMenu pauseMenu, victoryMenu;
    float gameOverTimer, startGameTimer = START_GAME_TIME, wallBlinkTimer = WALL_BLINK_TIME, victoryTimer = VICTORY_TIME;
    ArcadeText arcadeText;
    std::vector<MenuOption> pauseOptions, victoryOptions;

    StateManager &stateManager;

    GameState(sf::RenderWindow &window, std::string mapPath, StateManager &sm);
    bool getMap();
    void update(float elapsed);
    void bounds();
    void collisions(float elapsed);
    void doGraphics();
    void doUI();
    void drawScore(int x, int y, int score);
    void drawLives();
    void drawFruit(float x, float y, char fruit, float scaleFactor);
    void drawRecentFruits();
    void resetRound();
    void resetGame();
    void setGameOver();
    void drawGameOver();
    void nextLevel();
    void saveHighscore();
    void getHighscore();
    void handle(const sf::Event::KeyPressed &key);
    bool isWall(int x, int y);
    bool isCorner(int x, int y);
    bool isFruit(char tileType);
    int getLevel();
};