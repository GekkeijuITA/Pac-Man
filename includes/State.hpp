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

#include "../includes/PauseMenu.hpp"

#define GAME_OVER_TIME 3.f

struct TextureData
{
    sf::Texture texture;
    sf::Vector2f scale;
};

struct State
{
    sf::RenderWindow window;
    // Textures for the map
    std::map<int, TextureData> mapTextures;
    // Textures for the game
    std::map<int, TextureData> textures;
    PacMan pacman;
    std::vector<std::vector<char>> map;

    Blinky blinky;
    Pinky pinky;
    Inky inky;
    Clyde clyde;

    int lives, score, highscore, level, fruitCount;
    bool gameOver, pause;

    size_t maxFruits;
    std::deque<sf::Vector2i> recentFruits;
    std::vector<std::unique_ptr<Fruit>> fruits;
    std::vector<sf::Vector2i> fruitPositions;

    PauseMenu pauseMenu;
    float gameOverTimer;

    State(unsigned w, unsigned h, std::string title);
    bool getMap(std::string mapPath);
    void update(float elapsed);
    void bounds();
    void collisions(float elapsed);
    void doGraphics();
    void doUI();
    void drawChar(int x, int y, sf::Vector2i charPos);
    void drawScore(int x, int y, int score);
    void drawLives();
    void drawFruit(float x, float y, sf::Vector2i fruitPos, float scaleFactor);
    void drawRecentFruits();
    void resetRound();
    void setGameOver();
    void drawGameOver();
};