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

#define GAME_OVER_TIME 3.f
#define START_GAME_TIME 3.f
#define LIVES 3
#define VICTORY_LEVEL 3

struct StateManager;

struct TextureData
{
    sf::Texture texture;
    sf::Vector2f scale;
};

struct GameState
{
    sf::RenderWindow &window;
    // Textures for the map
    std::map<int, TextureData> mapTextures;
    // Textures for the game
    std::map<int, TextureData> textures;
    PacMan pacman;
    std::vector<std::vector<char>> map;
    std::string mapPath;
    std::vector<Ghost *> ghosts;

    Blinky blinky;
    Pinky pinky;
    Inky inky;
    Clyde clyde;

    int lives, score, highscore, level, fruitCount, eatableTiles;
    bool gameOver, pause, startGame, victory;

    size_t maxFruits;
    std::deque<sf::Vector2i> recentFruits;
    std::vector<std::unique_ptr<Fruit>> fruits;
    std::vector<sf::Vector2i> fruitPositions;

    GameMenu pauseMenu, victoryMenu;
    float gameOverTimer, startGameTimer;
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
    void drawFruit(float x, float y, sf::Vector2i fruitPos, float scaleFactor);
    void drawRecentFruits();
    void resetRound();
    void resetGame();
    void setGameOver();
    void drawGameOver();
    void nextLevel();
    void saveHighscore();
    void getHighscore();
};