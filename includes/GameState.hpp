#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <cmath>
#include <vector>
#include <deque>
#include <memory>

#include "../includes/PacMan.hpp"
#include "../includes/ghosts/Blinky.hpp"
#include "../includes/ghosts/Pinky.hpp"
#include "../includes/ghosts/Inky.hpp"
#include "../includes/ghosts/Clyde.hpp"

#include "../includes/fruits/Cherry.hpp"
#include "../includes/fruits/Orange.hpp"
#include "../includes/fruits/Strawberry.hpp"
#include "../includes/fruits/Grape.hpp"
#include "../includes/fruits/Apple.hpp"
#include "../includes/fruits/Bell.hpp"
#include "../includes/fruits/Key.hpp"
#include "../includes/fruits/Galaxian.hpp"

#include "../includes/GameMenu.hpp"

#define GAME_OVER_TIME 3.f
#define START_GAME_TIME 3.f

struct TextureData
{
    sf::Texture texture;
    sf::Vector2f scale;
};

struct GameState
{
    sf::RenderWindow window;
    // Textures for the map
    std::map<int, TextureData> mapTextures;
    // Textures for the game
    std::map<int, TextureData> textures;
    PacMan pacman;
    std::vector<std::vector<char>> map;
    std::string mapPath;

    Blinky blinky;
    Pinky pinky;
    Inky inky;
    Clyde clyde;

    int lives, score, highscore, level, fruitCount, eatableTiles;
    bool gameOver, pause, startGame;

    size_t maxFruits;
    std::deque<sf::Vector2i> recentFruits;
    std::vector<std::unique_ptr<Fruit>> fruits;
    std::vector<sf::Vector2i> fruitPositions;

    GameMenu pauseMenu;
    float gameOverTimer, startGameTimer;
    ArcadeText arcadeText;

    GameState(unsigned w, unsigned h, std::string title, std::string mapPath);
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
    void setGameOver();
    void drawGameOver();
    void nextLevel();
};