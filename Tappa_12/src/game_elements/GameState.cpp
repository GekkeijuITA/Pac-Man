#include "../../includes/lib/textures.hpp"
#include "../../includes/lib/global_values.hpp"
#include "../../includes/game_elements/GameState.hpp"
#include "../../includes/lib/text_ui.hpp"
#include "../../includes/core/Debug.hpp"
#include "../../includes/lib/TileFactory.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>

namespace std
{
    bool operator==(const sf::Vector2i &a, const sf::Vector2i &b)
    {
        return a.x == b.x && a.y == b.y;
    }
}

GameState::GameState(sf::RenderWindow &window, std::string mapPath, StateManager &sm) : pacman(*this),
                                                                                        blinky(*this),
                                                                                        pinky(*this),
                                                                                        inky(*this),
                                                                                        clyde(*this),
                                                                                        mapPath(mapPath),
                                                                                        window(window),
                                                                                        stateManager(sm)
{

    if (!wallBlinkTexture.loadFromFile(STRAIGHT_LINE_H_BLINK))
    {
        std::cerr << "Errore nel caricamento della texture del muro" << std::endl;
        exit(1);
    }

    if (!cornerBlinkTexture.loadFromFile(ANGLE_0_BLINK))
    {
        std::cerr << "Errore nel caricamento della texture dell'angolo" << std::endl;
        exit(1);
    }

    if (!getMap())
    {
        std::cerr << "Errore nel caricamento della mappa" << std::endl;
        exit(1);
    }

    eatenFruits.clear();

    pauseOptions = {
        {"CONTINUE",
         [this]()
         { pause = false; }},
        {"RESTART LEVEL", [this]()
         { resetRound(); }},
        {"MAIN MENU",
         [this]()
         {
             saveHighscore();
             stateManager.currentMode = StateManager::MAIN_MENU;
             stateManager.mainMenuState->getHighscore();
         }}};

    victoryOptions = {{"RESTART GAME", [this]()
                       { resetGame(); }},
                      {"MAIN MENU", [this]()
                       {
                           stateManager.currentMode = StateManager::MAIN_MENU;
                           stateManager.mainMenuState->getHighscore();
                       }}};
    pauseMenu = GameMenu(window.getView(), "PAUSE", sf::Vector2i(10, 3), TextColor::WHITE, pauseOptions, sf::Vector2i(10, 6));
    victoryMenu = GameMenu(window.getView(), "VICTORY", sf::Vector2i(8, 3), TextColor::WHITE, victoryOptions, sf::Vector2i(8, 6));

    getHighscore();

    ArcadeText arcadeText;

    pacman.setPowerPelletDuration();
    pacman.setSpeed();
}

void GameState::update(float elapsed)
{
    if (pause || victory)
        return;

    if (startGame)
    {
        if (startGameTimer > 0.f)
        {
            startGameTimer -= elapsed;
            return;
        }
        startGame = false;
    }

    if (gameOver)
    {
        if (gameOverTimer > 0.f)
        {
            gameOverTimer -= elapsed;
        }
        return;
    }

    if (pacman.getDotEaten() == eatableTiles)
    {
        isWallBlinking = true;
        if (victoryTimer > 0.f)
        {
            victoryTimer -= elapsed;
            if (wallBlinkTimer > 0.f)
            {
                wallBlinkTimer -= elapsed;
            }
            else
            {
                wallBlinkTimer = 0.2f;
                isWallWhite = !isWallWhite;
            }
        }
        else
        {
            nextLevel();
        }
    }

    if (!isWallBlinking)
    {
        if (!pacman.isDead)
        {
            if (pacman.powerPellet)
            {
                if (pacman.powerPelletDurationTimer > 0.f)
                {
                    pacman.powerPelletDurationTimer -= elapsed;
                }
                else
                {
                    pacman.powerPellet = false;
                    pacman.powerPelletDurationTimer = pacman.powerPelletDuration;
                    pacman.currentSpeed = pacman.speed;

                    for (Ghost *ghost : ghosts)
                    {
                        if (ghost->state == Ghost::SCARED)
                        {
                            ghost->setState(ghost->lastState);
                            ghost->isTransitioning = (ghost->lastState == Ghost::IN_HOUSE);
                        }
                    }
                }
            }

            collisions(elapsed);

            for (Ghost *ghost : ghosts)
            {
                ghost->move(elapsed);
                if (ghost->state == Ghost::EATEN)
                {
                    if (ghost->scoreDisplayTimer > 0.f)
                        ghost->scoreDisplayTimer -= elapsed;
                }

                if (ghost->state == Ghost::SCARED)
                {
                    if (pacman.powerPelletDurationTimer <= pacman.powerPelletDuration * .4f)
                    {
                        if (ghost->blinkingTime > 0.f)
                        {
                            ghost->blinkingTime -= elapsed;
                            if (ghost->isWhite)
                            {
                                ghost->backScaredAnim->update(elapsed);
                            }
                            else
                            {
                                ghost->scaredAnim->update(elapsed);
                            }
                        }
                        else
                        {
                            ghost->blinkingTime = 0.2f;
                            ghost->isWhite = !ghost->isWhite;
                        }
                    }
                    else
                        ghost->scaredAnim->update(elapsed);
                }
            }

            for (auto &fruitData : fruits)
            {
                if (fruitData.second.fruit->fruitDisplayTimer > 0.f)
                {
                    if (!fruitData.second.isEaten)
                    {
                        if (!pacman.powerPellet)
                            fruitData.second.fruit->fruitDisplayTimer -= elapsed;

                        if (fruitData.second.fruit->fruitDisplayTimer <= 3.f)
                        {
                            if (fruitData.second.fruit->blinkingTime > 0.f)
                            {
                                fruitData.second.fruit->blinkingTime -= elapsed;
                            }
                            else
                            {
                                fruitData.second.fruit->isBlinking = !fruitData.second.fruit->isBlinking;
                                fruitData.second.fruit->blinkingTime = FRUIT_BLINKING_TIME;
                            }
                        }
                    }
                }
                else if (fruitData.second.fruit->scoreDisplayTimer > 0.f && fruitData.second.isEaten)
                {
                    fruitData.second.fruit->scoreDisplayTimer -= elapsed;

                    if (fruitData.second.fruit->scoreDisplayTimer < 0.f)
                    {
                        fruitData.second.isVisible = false;
                        fruitData.second.isEaten = false;
                    }
                }
                else
                {
                    fruitData.second.isVisible = false;
                    fruitData.second.isEaten = false;
                    fruitData.second.fruit->isBlinking = false;
                }

                if ((pacman.getDotEaten() == 70 || pacman.getDotEaten() == 170) && !fruitData.second.isVisible)
                {
                    fruitData.second.fruit->setTimer();
                    fruitData.second.isVisible = true;
                }
            }

            bounds();
        }
        else
        {
            if (!pacman.DEATH_ANIMATION.empty())
            {
                pacman.DEATH_ANIMATION.front().update(elapsed);

                if (pacman.DEATH_ANIMATION.front().isFinished())
                {
                    pacman.DEATH_ANIMATION.clear();
                    pacman.isDead = false;
                    resetRound();
                }
            }
        }
    }
}

void checkBounds(sf::Vector2i &pos)
{
    if (pos.x < 0)
    {
        pos.x = MAP_HEIGHT;
    }
    else if (pos.x > MAP_HEIGHT)
    {
        pos.x = 0;
    }

    if (pos.y < 0)
    {
        pos.y = MAP_WIDTH;
    }
    else if (pos.y > MAP_WIDTH)
    {
        pos.y = 0;
    }
}

void GameState::bounds()
{
    checkBounds(pacman.position);

    for (Ghost *ghost : ghosts)
    {
        checkBounds(ghost->position);
    }
}

void GameState::collisions(float elapsed)
{
    int next_x = pacman.position.x;
    int next_y = pacman.position.y;

    pacman.updateDirection();

    switch (pacman.direction)
    {
    case UP:
        next_x--;
        break;
    case DOWN:
        next_x++;
        break;
    case LEFT:
        next_y--;
        break;
    case RIGHT:
        next_y++;
        break;
    default:
        break;
    }

    if (!pacman.isWall(next_x, next_y))
    {
        pacman.move(elapsed);
    }

    for (auto &fruitData : fruits)
    {
        if (fruitData.second.fruit->position == pacman.position && !fruitData.second.isEaten && fruitData.second.fruit->fruitDisplayTimer > 0.f)
        {
            int recentFruitsSize = eatenFruits.size();
            if (recentFruitsSize > maxFruits)
            {
                eatenFruits.pop_front();
                recentFruitsSize--;
            }

            score += fruitData.second.fruit->getScore();
            eatenFruits.push_back(fruitData.first);
            fruitData.second.fruit->fruitDisplayTimer = 0.f;
            fruitData.second.isEaten = true;
            fruitData.second.fruit->scoreDisplayTimer = SCORE_DISPLAY_TIME;
        }
    }
}

bool GameState::getMap()
{
    eatableTiles = 0;
    ghosts.clear();
    std::fstream mapFile;
    mapFile.open(mapPath, std::ios::in);
    if (!mapFile.is_open())
    {
        std::cerr << "Errore nell'apertura del file contenente la mappa" << std::endl;
        return false;
    }

    std::string mapString;

    map.clear();

    while (std::getline(mapFile, mapString))
    {
        std::vector<char> row(mapString.begin(), mapString.end());
        map.push_back(row);

        for (int i = 0; i < row.size(); i++)
        {
            if (row[i] == PACMAN)
            {
                pacman.setPosition(map.size() - 1, i);
                pacman.spawn = {static_cast<int>(map.size()) - 1, i};
                map.back()[i] = EMPTY_BLOCK;
            }
            else if (row[i] == BLINKY)
            {
                blinky.setPosition(map.size() - 1, i);
                blinky.spawn = {static_cast<int>(map.size()) - 1, i};
                map.back()[i] = EMPTY_BLOCK;
                ghosts.push_back(&blinky);
            }
            else if (row[i] == PINKY)
            {
                pinky.setPosition(map.size() - 1, i);
                pinky.spawn = {static_cast<int>(map.size()) - 1, i};
                map.back()[i] = EMPTY_BLOCK;
                ghosts.push_back(&pinky);
            }
            else if (row[i] == INKY)
            {
                inky.setPosition(map.size() - 1, i);
                inky.spawn = {static_cast<int>(map.size()) - 1, i};
                map.back()[i] = EMPTY_BLOCK;
                ghosts.push_back(&inky);
            }
            else if (row[i] == CLYDE)
            {
                clyde.setPosition(map.size() - 1, i);
                clyde.spawn = {static_cast<int>(map.size()) - 1, i};
                map.back()[i] = EMPTY_BLOCK;
                ghosts.push_back(&clyde);
            }
            else if (row[i] == GHOST_DOOR_H || row[i] == GHOST_DOOR_V)
            {
                blinky.addExitTile(map.size() - 1, i);
                pinky.addExitTile(map.size() - 1, i);
                inky.addExitTile(map.size() - 1, i);
                clyde.addExitTile(map.size() - 1, i);
            }
            else if (row[i] == FRUIT)
            {
                sf::Vector2i fruitPos = {static_cast<int>(map.size()) - 1, i};
                switch (level)
                {
                case 1:
                    fruits.insert(std::make_pair(CHERRY, FruitData{fruitPos, std::make_unique<Cherry>(fruitPos), false}));
                    break;
                case 2:
                    fruits.insert(std::make_pair(STRAWBERRY, FruitData{fruitPos, std::make_unique<Strawberry>(fruitPos), false}));
                    break;
                case 3:
                    fruits.insert(std::make_pair(ORANGE_FRUIT, FruitData{fruitPos, std::make_unique<Orange>(fruitPos), false}));
                    break;
                case 4:
                    fruits.insert(std::make_pair(APPLE, FruitData{fruitPos, std::make_unique<Apple>(fruitPos), false}));
                    break;
                case 5:
                    fruits.insert(std::make_pair(GRAPE, FruitData{fruitPos, std::make_unique<Grape>(fruitPos), false}));
                    break;
                case 6:
                    fruits.insert(std::make_pair(GALAXIAN, FruitData{fruitPos, std::make_unique<Galaxian>(fruitPos), false}));
                    break;
                case 7:
                    fruits.insert(std::make_pair(BELL, FruitData{fruitPos, std::make_unique<Bell>(fruitPos), false}));
                    break;
                default:
                    fruits.insert(std::make_pair(KEY, FruitData{fruitPos, std::make_unique<Key>(fruitPos), false}));
                    break;
                }
            }
            else if (row[i] == PACDOT || row[i] == POWERPELLET)
            {
                eatableTiles++;
            }
        }
    }
    mapFile.close();

    pacman.setMap(&map);

    for (Ghost *ghost : ghosts)
    {
        ghost->setMap(&map);
        ghost->setSpeed();
    }

    return true;
}

void GameState::doGraphics()
{
    for (int r = 0; r < MAP_HEIGHT; r++)
    {
        float y = (r + 3) * TILE_SIZE; // Lasciamo le prime due righe per il punteggio
        for (int c = 0; c < MAP_WIDTH; c++)
        {
            float x = c * TILE_SIZE;
            char tileType = map[r][c];
            if (tileType != EMPTY_BLOCK)
            {
                auto tile = TileFactory::getIstance().getTile(tileType);
                if (!tile)
                {
                    std::cerr << "Error loading tile data" << std::endl;
                    break;
                }

                if (tileType == FRUIT)
                {
                    continue;
                }

                if (isWallBlinking)
                {
                    if (isWall(r, c))
                    {
                        if (isWallWhite)
                        {
                            tile->sprite.setTexture(wallBlinkTexture);
                        }
                    }
                    else if (isCorner(r, c))
                    {
                        if (isWallWhite)
                        {
                            tile->sprite.setTexture(cornerBlinkTexture);
                        }
                    }
                }

                tile->sprite.setPosition({static_cast<float>(x) + TILE_SIZE / 2.f, static_cast<float>(y) + TILE_SIZE / 2.f});
                tile->sprite.setScale({(float)TILE_SIZE / tile->sprite.getTexture().getSize().x, (float)TILE_SIZE / tile->sprite.getTexture().getSize().y});
                window.draw(tile->sprite);
            }
        }

        if (!gameOver)
        {
            pacman.draw(window);

            if (!pacman.isDead && !isWallBlinking)
            {
                for (Ghost *ghost : ghosts)
                {
                    ghost->draw(window);

                    if (ghost->scoreDisplayTimer > 0.f)
                    {
                        ghost->drawScore();
                    }
                    else
                    {
                        ghost->stoppedForScore = false;
                    }
                }

                for (const auto &fruitElement : fruits)
                {
                    if (fruitElement.second.fruit->fruitDisplayTimer > 0.f && !fruitElement.second.isEaten)
                    {
                        if (!fruitElement.second.fruit->isBlinking)
                            fruitElement.second.fruit->draw(window, fruitElement.first);
                    }
                    else if (fruitElement.second.fruit->scoreDisplayTimer > 0.f && fruitElement.second.isEaten)
                    {
                        fruitElement.second.fruit->drawScore(window);
                    }
                }
            }
        }
    }

    // Debug::drawGrid(window);
}

void GameState::doUI()
{

    if (startGame && !gameOver)
    {
        arcadeText.drawString("READY!", 11, 20, window, TextColor::YELLOW);
    }

    drawLives();

    arcadeText.drawString("1UP", 3, 0, window, TextColor::WHITE);
    arcadeText.drawString("HIGH SCORE", 9, 0, window, TextColor::WHITE);

    drawScore(7, 1, score);

    if (score > highscore)
    {
        drawScore(17, 1, score);
    }
    else
    {
        drawScore(17, 1, highscore);
    }

    drawRecentFruits();

    if (pause)
    {
        pauseMenu.draw(window);
    }

    if (gameOver)
    {
        drawGameOver();
    }

    if (victory)
    {
        victoryMenu.draw(window);
    }
}

void GameState::drawScore(int x, int y, int score)
{
    std::string scoreString = std::to_string(score);
    int scoreLength = scoreString.length();
    int startX = x - scoreLength;

    arcadeText.drawString(scoreString, startX, y, window, TextColor::WHITE);
}

void GameState::drawLives()
{
    auto tile = TileFactory::getIstance().getTile(PACMAN);
    if (!tile)
    {
        std::cerr << "Error loading tile data" << std::endl;
        return;
    }

    for (int i = 0; i < lives; i++)
    {
        tile->sprite.setPosition({
            (((i + 1) * 2) + 1.f) * TILE_SIZE,
            (MAP_HEIGHT + 4) * TILE_SIZE,
        });
        tile->sprite.setScale({4.f, 4.f});
        window.draw(tile->sprite);
    }
}

void GameState::drawFruit(float x, float y, char fruit, float scaleFactor)
{
    auto tile = TileFactory::getIstance().getTile(fruit);
    if (!tile)
    {
        std::cerr << "Error loading tile data for fruit: " << fruit << std::endl;
        return;
    }
    tile->sprite.setPosition({x * TILE_SIZE, y * TILE_SIZE});
    tile->sprite.setScale({scaleFactor, scaleFactor});
    window.draw(tile->sprite);
}

void GameState::drawRecentFruits()
{
    int recentFruitsSize = eatenFruits.size();
    int startX = 24 - (recentFruitsSize - 1) * 2;

    for (size_t i = 0; i < recentFruitsSize; i++)
    {
        drawFruit(startX + (i * 2) + 1.f, MAP_HEIGHT + 4, eatenFruits[i], 4.f);
    }
}

void GameState::resetRound()
{
    pause = false;
    startGame = true;
    startGameTimer = START_GAME_TIME;
    victoryTimer = VICTORY_TIME;
    wallBlinkTimer = WALL_BLINK_TIME;
    isWallBlinking = false;
    isWallWhite = false;

    pacman.respawn();
    for (Ghost *ghost : ghosts)
    {
        if (ghost == &blinky)
        {
            ghost->respawn(Ghost::NORMAL);
        }
        else
        {
            ghost->respawn(Ghost::IN_HOUSE);
        }
    }

    if (lives <= 0 && !gameOver && !pacman.isDead)
    {
        setGameOver();
        return;
    }

    pauseMenu.setCursorPosition(pauseMenu.startOptionsPos.x - 1, pauseMenu.startOptionsPos.y);
    pauseMenu.cursorIndex = 0;
}

void GameState::resetGame()
{
    victory = false;
    lives = LIVES;
    gameOver = false;
    gameOverTimer = GAME_OVER_TIME;
    score = 0;
    level = 1;
    eatenFruits.clear();
    map.clear();
    fruits.clear();
    getMap();
    resetRound();
    pacman.dotEaten = 0;
}

void GameState::setGameOver()
{
    gameOver = true;
    gameOverTimer = GAME_OVER_TIME;
    saveHighscore();
}

void GameState::drawGameOver()
{
    arcadeText.drawString("GAME OVER", 9, 20, window, TextColor::RED);

    if (gameOverTimer <= 0.f)
    {
        arcadeText.drawString("PRESS ENTER TO", 7, 22, window, TextColor::RED);
        arcadeText.drawString("RETURN TO THE MENU", 5, 23, window, TextColor::RED);
    }
}

void GameState::nextLevel()
{
    saveHighscore();

    if (level >= VICTORY_LEVEL)
    {
        victory = true;
        return;
    }

    level++;
    map.clear();
    pacman.dotEaten = 0;
    fruits.clear();
    getMap();
    resetRound();
    getHighscore();
}

void GameState::saveHighscore()
{
    std::ofstream highscoreFile;
    highscoreFile.open("../Tappa_12/resources/highscore.txt", std::ios::out);
    if (!highscoreFile.is_open())
    {
        std::cerr << "Errore nell'apertura del file contenente l'highscore" << std::endl;
        return;
    }
    if (score > highscore)
    {
        highscoreFile << score;
    }
    highscoreFile.close();
}

void GameState::getHighscore()
{
    std::ifstream highscoreFile;
    highscoreFile.open("../Tappa_12/resources/highscore.txt", std::ios::in);
    if (!highscoreFile.is_open())
    {
        std::cerr << "Errore nell'apertura del file contenente l'highscore" << std::endl;
        return;
    }
    std::string highscoreString;
    std::getline(highscoreFile, highscoreString);
    highscore = highscoreString.empty() ? 0 : std::stoi(highscoreString);
    highscoreFile.close();
}

bool GameState::isWall(int x, int y)
{
    if (x < 0 || x >= MAP_HEIGHT || y < 0 || y >= MAP_WIDTH)
        return false;

    char tile = map[x][y];
    return tile == LINE_H || tile == LINE_V;
}

bool GameState::isCorner(int x, int y)
{
    if (x < 0 || x >= MAP_HEIGHT || y < 0 || y >= MAP_WIDTH)
        return false;

    char tile = map[x][y];
    return tile == CORNER_0 || tile == CORNER_90 || tile == CORNER_180 || tile == CORNER_270;
}

bool GameState::isFruit(char tileType)
{
    return tileType == FRUIT || tileType == CHERRY || tileType == STRAWBERRY || tileType == ORANGE_FRUIT ||
           tileType == APPLE || tileType == GRAPE || tileType == GALAXIAN || tileType == BELL || tileType == KEY;
}

void GameState::handle(const sf::Event::KeyPressed &key)
{
    if (pause)
    {
        pauseMenu.handle(key);
        return;
    }

    if (victory)
    {
        victoryMenu.handle(key);
        return;
    }

    if (gameOver)
    {
        if (key.scancode == sf::Keyboard::Scancode::Enter && gameOverTimer <= 0.f)
        {
            stateManager.currentMode = StateManager::MAIN_MENU;
            stateManager.mainMenuState->getHighscore();
        }
        return;
    }

    Direction newDirection = pacman.direction;

    switch (key.scancode)
    {
    case sf::Keyboard::Scancode::Up:
    {
        if (!pause)
            newDirection = UP;
        break;
    }
    case sf::Keyboard::Scancode::Down:
    {
        if (!pause)
            newDirection = DOWN;
        break;
    }
    case sf::Keyboard::Scancode::Left:
    {
        if (!pause)
            newDirection = LEFT;
        break;
    }
    case sf::Keyboard::Scancode::Right:
    {
        if (!pause)
            newDirection = RIGHT;
        break;
    }
    case sf::Keyboard::Scancode::Escape:
    {
        if (!gameOver)
        {
            pause = !pause;
            pauseMenu.cursorIndex = 0;
        }
        break;
    }
    default:
        return;
    }

    pacman.setNextDirection(newDirection);
}