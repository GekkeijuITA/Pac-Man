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
#include "../LevelSelectorState.hpp"

#define GAME_OVER_TIME 3.f
#define DEATH_TIME 2.2f
#define START_GAME_TIME 4.1f
#define LIVES 3
#define VICTORY_LEVEL 256
#define WALL_BLINK_TIME 0.3f
#define VICTORY_TIME 3.f

struct StateManager;
struct Fruit;
struct LevelSelectorState;

struct TextureData
{
    sf::Texture texture;
    sf::Vector2f scale;
};

struct FruitData
{
    sf::Vector2i position;
    std::unique_ptr<Fruit> fruit;
};

struct GameState
{
    enum Mode
    {
        MENU,
        IN_GAME,
        PLAY_MAP,
        PLAY_SCENARIO
    };

    Mode currentMode = MENU;

    sf::RenderWindow &window;
    bool gameOver = false, pause = false, startGame = true, victory = false, isWallBlinking = false, isWallWhite = false;
    bool loop = false;

    PacMan pacman;
    std::vector<std::vector<char>> map;
    std::vector<std::string> mapsPath;
    std::vector<Ghost *> ghosts;
    sf::Texture wallBlinkTexture, cornerBlinkTexture;

    Blinky blinky;
    Pinky pinky;
    Inky inky;
    Clyde clyde;

    int lives = LIVES, score = 0, highscore = 0, level = 1, eatableTiles = 0;

    size_t maxFruits = 11 - LIVES;
    std::deque<char> eatenFruits;
    std::multimap<char, FruitData> fruits;

    float gameOverTimer, startGameTimer = START_GAME_TIME, wallBlinkTimer = WALL_BLINK_TIME, victoryTimer = VICTORY_TIME;
    ArcadeText arcadeText;
    std::vector<MenuOption> pauseOptions = {},
                            victoryOptions = {},
                            mainMenuOptions = {},
                            scenarios = {};
    GameMenu pauseMenu, victoryMenu, mainMenu, scenariosList;

    std::unique_ptr<LevelSelectorState> levelSelectorState;

    StateManager &stateManager;

    GameState(sf::RenderWindow &window, StateManager &sm);
    bool getMap(std::string mapPath);
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
    void handle(const sf::Event::MouseMoved &mouse);
    void handle(const sf::Event::MouseButtonPressed &mouse);
    void handle(const sf::Event::MouseWheelScrolled &wheel);
    bool isWall(int x, int y);
    bool isCorner(int x, int y);
    bool isFruit(char tileType);
    int getLevel();
    void manageSounds();
    void loadMap(std::string mapPath);
    void loadScenario(std::string scenarioPath);
    void initScenarioList();
};