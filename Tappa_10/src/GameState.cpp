#include "../includes/textures.hpp"
#include "../includes/global_values.hpp"
#include "../includes/GameState.hpp"
#include "../includes/text_ui.hpp"
#include "../includes/Debug.hpp"

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

GameState::GameState(sf::RenderWindow &window, std::string mapPath, StateManager &stateManager) : lives(LIVES),
                                                                                                  score(0),
                                                                                                  highscore(0),
                                                                                                  pacman(*this),
                                                                                                  blinky(*this),
                                                                                                  pinky(*this),
                                                                                                  inky(*this),
                                                                                                  clyde(*this),
                                                                                                  eatableTiles(0),
                                                                                                  gameOver(false),
                                                                                                  pause(false),
                                                                                                  startGame(true),
                                                                                                  level(1),
                                                                                                  fruitCount(0),
                                                                                                  startGameTimer(START_GAME_TIME),
                                                                                                  mapPath(mapPath),
                                                                                                  victory(false),
                                                                                                  window(window),
                                                                                                  stateManager(stateManager)
{
    if (!getMap())
    {
        std::cerr << "Errore nel caricamento della mappa" << std::endl;
        exit(1);
    }

    recentFruits.clear();

    pauseMenu = GameMenu(window.getView(), "PAUSE", {"CONTINUE", "RESTART LEVEL", "MAIN MENU"}, sf::Vector2i(10, 3));
    victoryMenu = GameMenu(window.getView(), "VICTORY", {"RESTART GAME", "MAIN MENU"}, sf::Vector2i(8, 3));

    sf::Texture temp;
    sf::Vector2u texSize;

    if (!temp.loadFromFile(STRAIGHT_LINE_H))
    {
        std::cerr << "Errore nel caricamento della texture" << std::endl;
        exit(1);
    }
    mapTextures[0].texture = temp;
    texSize = mapTextures[0].texture.getSize();
    mapTextures[0].scale = {(float)TILE_SIZE / texSize.x, (float)TILE_SIZE / texSize.y};

    if (!temp.loadFromFile(ANGLE_0))
    {
        std::cerr << "Errore nel caricamento della texture" << std::endl;
        exit(1);
    }
    mapTextures[1].texture = temp;
    texSize = mapTextures[1].texture.getSize();
    mapTextures[1].scale = {(float)TILE_SIZE / texSize.x, (float)TILE_SIZE / texSize.y};

    if (!temp.loadFromFile(ASSET))
    {
        std::cerr << "Errore nel caricamento della texture" << std::endl;
        exit(1);
    }
    textures[0].texture = temp;
    texSize = textures[0].texture.getSize();
    textures[0].scale = {2.f, 2.f};

    if (!temp.loadFromFile(TEXT))
    {
        std::cerr << "Errore nel caricamento della texture" << std::endl;
        exit(1);
    }
    textures[1].texture = temp;
    texSize = textures[1].texture.getSize();
    textures[1].scale = {(float)TILE_SIZE / texSize.x, (float)TILE_SIZE / texSize.y};

    maxFruits = 12 - lives;
    fruitCount = 0;
    getHighscore();

    ArcadeText arcadeText;
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
        nextLevel();
    }

    if (pacman.powerPellet)
    {
        pacman.powerPelletDuration -= elapsed;
        if (pacman.powerPelletDuration <= 0.f)
        {
            pacman.powerPellet = false;

            for (Ghost *ghost : std::initializer_list<Ghost *>{&blinky, &pinky, &inky, &clyde})
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

    for (Ghost *ghost : std::initializer_list<Ghost *>{&blinky, &pinky, &inky, &clyde})
    {
        ghost->move(elapsed);
        if (ghost->state == Ghost::EATEN)
        {
            if (ghost->scoreDisplayTimer > 0.f)
                ghost->scoreDisplayTimer -= elapsed;
        }

        if (ghost->state == Ghost::SCARED)
        {
            if (pacman.powerPelletDuration <= 3.f)
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

    for (auto &fruit : fruits)
    {
        bool isFruitDisplayed = find(fruitPositions.begin(), fruitPositions.end(), fruit->position) != fruitPositions.end();

        if (fruit->fruitDisplayTimer > 0.f && !fruit->eaten)
        {
            if (!isFruitDisplayed)
            {
                fruitPositions.push_back(fruit->position);
            }

            if (!pacman.powerPellet)
                fruit->fruitDisplayTimer -= elapsed;

            if (fruit->fruitDisplayTimer <= 3.f)
            {
                if (fruit->blinkingTime > 0.f)
                {
                    fruit->blinkingTime -= elapsed;
                }
                else
                {
                    fruit->isBlinking = !fruit->isBlinking;
                    fruit->blinkingTime = FRUIT_BLINKING_TIME;
                }
            }
        }

        if (fruit->scoreDisplayTimer > 0.f && fruit->eaten)
        {
            fruit->scoreDisplayTimer -= elapsed;

            if (fruit->scoreDisplayTimer <= 0.f)
            {
                isFruitDisplayed = false;
                fruitPositions.erase(std::remove(fruitPositions.begin(), fruitPositions.end(), fruit->position), fruitPositions.end());
                fruit->fruitDisplayTimer = 0.f;
                fruit->eaten = false;
                fruit->scoreDisplayTimer = 0.f;
            }
        }

        if ((pacman.getDotEaten() == 70 || pacman.getDotEaten() == 170) && !isFruitDisplayed)
        {
            fruit->setTimer();
        }
    }

    bounds();
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

    for (Ghost *ghost : std::initializer_list<Ghost *>{&blinky, &pinky, &inky, &clyde})
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

    for (auto &fruit : fruits)
    {
        if (fruit->fruitDisplayTimer > 0.f && fruit->position == pacman.position)
        {
            score += fruit->getScore();
            fruitCount++;
            recentFruits.push_back(fruit->texPosition);
            fruit->fruitDisplayTimer = 0.f;
            fruit->eaten = true;
            fruit->scoreDisplayTimer = SCORE_DISPLAY_TIME;
        }
    }
}

bool GameState::getMap()
{
    eatableTiles = 0;
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
            }
            else if (row[i] == PINKY)
            {
                pinky.setPosition(map.size() - 1, i);
                pinky.spawn = {static_cast<int>(map.size()) - 1, i};
                map.back()[i] = EMPTY_BLOCK;
            }
            else if (row[i] == INKY)
            {
                inky.setPosition(map.size() - 1, i);
                inky.spawn = {static_cast<int>(map.size()) - 1, i};
                map.back()[i] = EMPTY_BLOCK;
            }
            else if (row[i] == CLYDE)
            {
                clyde.setPosition(map.size() - 1, i);
                clyde.spawn = {static_cast<int>(map.size()) - 1, i};
                map.back()[i] = EMPTY_BLOCK;
            }
            else if (row[i] == GHOST_DOOR)
            {
                blinky.addExitTile(map.size() - 1, i);
                pinky.addExitTile(map.size() - 1, i);
                inky.addExitTile(map.size() - 1, i);
                clyde.addExitTile(map.size() - 1, i);
            }
            else if (row[i] == FRUIT)
            {
                switch (level)
                {
                case 1:
                    fruits.push_back(std::make_unique<Cherry>(sf::Vector2i(map.size() - 1, i)));
                    break;
                case 2:
                    fruits.push_back(std::make_unique<Strawberry>(sf::Vector2i(map.size() - 1, i)));
                    break;
                case 3:
                    fruits.push_back(std::make_unique<Orange>(sf::Vector2i(map.size() - 1, i)));
                    break;
                case 4:
                    fruits.push_back(std::make_unique<Apple>(sf::Vector2i(map.size() - 1, i)));
                    break;
                case 5:
                    fruits.push_back(std::make_unique<Grape>(sf::Vector2i(map.size() - 1, i)));
                    break;
                case 6:
                    fruits.push_back(std::make_unique<Galaxian>(sf::Vector2i(map.size() - 1, i)));
                    break;
                case 7:
                    fruits.push_back(std::make_unique<Bell>(sf::Vector2i(map.size() - 1, i)));
                    break;
                default:
                    fruits.push_back(std::make_unique<Key>(sf::Vector2i(map.size() - 1, i)));
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

    for (Ghost *ghost : std::initializer_list<Ghost *>{&blinky, &pinky, &inky, &clyde})
    {
        ghost->setMap(&map);
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
            switch (map[r][c])
            {
            case PACDOT:
            {
                sf::CircleShape pacdot(TILE_SIZE / 10.f);
                pacdot.setPosition({x + (TILE_SIZE / 2) - 4, y + (TILE_SIZE / 2) - 4});
                pacdot.setFillColor(sf::Color(255, 185, 176));
                window.draw(pacdot);
                break;
            }
            case LINE_V:
            {
                sf::Sprite wall(mapTextures[0].texture);
                wall.setScale(mapTextures[0].scale);
                wall.setPosition({x, y});
                window.draw(wall);
                break;
            }
            case LINE_H:
            {
                sf::Sprite wall(mapTextures[0].texture);
                wall.setScale(mapTextures[0].scale);
                wall.setOrigin({mapTextures[0].texture.getSize().x / 2.f,
                                mapTextures[0].texture.getSize().y / 2.f});
                wall.setPosition({x + TILE_SIZE / 2.f, y + TILE_SIZE / 2.f});
                wall.setRotation(sf::degrees(-90));
                window.draw(wall);
                break;
            }
            case CORNER_0:
            {
                sf::Sprite wall(mapTextures[1].texture);
                wall.setScale(mapTextures[1].scale);
                wall.setPosition({x, y});
                window.draw(wall);
                break;
            }
            case CORNER_90:
            {
                sf::Sprite wall(mapTextures[1].texture);
                wall.setScale(mapTextures[1].scale);
                wall.setOrigin({mapTextures[1].texture.getSize().x / 2.f,
                                mapTextures[1].texture.getSize().y / 2.f});

                wall.setPosition({x + TILE_SIZE / 2.f, y + TILE_SIZE / 2.f});
                wall.setRotation(sf::degrees(-90));
                window.draw(wall);
                break;
            }
            case CORNER_180:
            {
                sf::Sprite wall(mapTextures[1].texture);
                wall.setScale(mapTextures[1].scale);
                wall.setOrigin({mapTextures[1].texture.getSize().x / 2.f,
                                mapTextures[1].texture.getSize().y / 2.f});

                wall.setPosition({x + TILE_SIZE / 2.f, y + TILE_SIZE / 2.f});
                wall.setRotation(sf::degrees(-180));
                window.draw(wall);
                break;
            }
            case CORNER_270:
            {
                sf::Sprite wall(mapTextures[1].texture);
                wall.setScale(mapTextures[1].scale);
                wall.setOrigin({mapTextures[1].texture.getSize().x / 2.f,
                                mapTextures[1].texture.getSize().y / 2.f});

                wall.setPosition({x + TILE_SIZE / 2.f, y + TILE_SIZE / 2.f});
                wall.setRotation(sf::degrees(-270));
                window.draw(wall);
                break;
            }
            case POWERPELLET:
            {
                float scale = 3.f;
                sf::CircleShape powerpellet(TILE_SIZE / scale);
                powerpellet.setOrigin({TILE_SIZE / scale, TILE_SIZE / scale});
                powerpellet.setPosition({x + (TILE_SIZE / 2), y + (TILE_SIZE / 2)});
                powerpellet.setFillColor(sf::Color(255, 185, 176));
                window.draw(powerpellet);
                break;
            }
            case GHOST_DOOR:
            {
                sf::RectangleShape ghostDoor({TILE_SIZE, TILE_SIZE / 4});
                ghostDoor.setPosition({x, y + TILE_SIZE / 1.8f});
                ghostDoor.setFillColor(sf::Color(255, 203, 255));
                window.draw(ghostDoor);
                break;
            }
            case EMPTY_BLOCK:
            {
                sf::RectangleShape emptyBlock({TILE_SIZE, TILE_SIZE});
                emptyBlock.setPosition({x, y});
                emptyBlock.setFillColor(sf::Color::Black);
                window.draw(emptyBlock);
                break;
            }
            default:
                break;
            }
        }

        if (!gameOver)
        {
            pacman.draw(window);

            for (Ghost *ghost : std::initializer_list<Ghost *>{&blinky, &pinky, &inky, &clyde})
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

            for (const auto &fruit : fruits)
            {
                if (fruit->fruitDisplayTimer > 0.f && !fruit->eaten)
                {
                    if (!fruit->isBlinking)
                        fruit->draw(window);
                }
                else if (fruit->scoreDisplayTimer > 0.f && fruit->eaten)
                {
                    fruit->drawScore(window);
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
        arcadeText.drawString("READY!", 11, 20, window, sf::Vector2i(0, 24));
    }

    drawLives();

    arcadeText.drawString("1UP", 3, 0, window);
    arcadeText.drawString("HIGH SCORE", 9, 0, window);

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

    arcadeText.drawString(scoreString, startX, y, window);
}

void GameState::drawLives()
{
    sf::Vector2i pacmanPos = pacman.PACMAN_TEX_MAP.at(LEFT);
    sf::Sprite pacmanSprite = createSprite(textures[0].texture, pacmanPos, textures[0].scale, 2.f, TILE_SIZE / 2, true);
    for (int i = 0; i < lives; i++)
    {
        pacmanSprite.setPosition({
            (((i + 1) * 2) + 1.f) * TILE_SIZE,
            (MAP_HEIGHT + 4) * TILE_SIZE,
        });
        window.draw(pacmanSprite);
    }
}

void GameState::drawFruit(float x, float y, sf::Vector2i fruitPos, float scaleFactor)
{
    sf::Sprite fruitSprite = createSprite(textures[0].texture, fruitPos, textures[0].scale, scaleFactor, TILE_SIZE / 2, true);
    fruitSprite.setPosition({x * TILE_SIZE, y * TILE_SIZE});
    window.draw(fruitSprite);
}

void GameState::drawRecentFruits()
{
    int recentFruitsSize = recentFruits.size();

    if (recentFruitsSize > maxFruits)
    {
        recentFruits.pop_front();
        recentFruitsSize--;
    }

    int startX = 24 - (recentFruitsSize - 1) * 2;

    for (size_t i = 0; i < recentFruits.size(); i++)
    {
        sf::Vector2i fruitPos = recentFruits[i];
        drawFruit((startX + (i * 2)) + 1.f, (MAP_HEIGHT + 4), fruitPos, 2.f);
    }
}

void GameState::resetRound()
{
    pause = false;
    startGame = true;
    startGameTimer = START_GAME_TIME;
    pacman.respawn();
    map.clear();
    fruitPositions.clear();
    fruits.clear();
    for (Ghost *ghost : std::initializer_list<Ghost *>{&blinky, &pinky, &inky, &clyde})
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

    if (lives <= 0 && !gameOver)
    {
        setGameOver();
    }

    getMap();
}

void GameState::resetGame()
{
    victory = false;
    lives = LIVES;
    score = 0;
    level = 1;
    fruitCount = 0;
    recentFruits.clear();
    resetRound();
}

void GameState::setGameOver()
{
    gameOver = true;
    gameOverTimer = GAME_OVER_TIME;
    saveHighscore();
}

void GameState::drawGameOver()
{
    arcadeText.drawString("GAME OVER", 9, 20, window, sf::Vector2i(0, 4));

    if (gameOverTimer <= 0.f)
    {
        arcadeText.drawString("PRESS ENTER TO", 7, 22, window, sf::Vector2i(0, 4));
        arcadeText.drawString("RETURN TO THE MENU", 5, 23, window, sf::Vector2i(0, 4));
    }
}

void GameState::nextLevel()
{
    saveHighscore();

    if (level == VICTORY_LEVEL)
    {
        victory = true;
        return;
    }

    resetRound();
    level++;
    getHighscore();
}

void GameState::saveHighscore()
{
    std::ofstream highscoreFile;
    highscoreFile.open("../Tappa_10/resources/highscore.txt", std::ios::out);
    if (!highscoreFile.is_open())
    {
        std::cerr << "Errore nell'apertura del file contenente l'highscore" << std::endl;
        return;
    }
    if (score > highscore)
    {
        // std::cout << "Nuovo highscore: " << score << std::endl;
        highscoreFile << score;
    }
    highscoreFile.close();
}

void GameState::getHighscore()
{
    std::ifstream highscoreFile;
    highscoreFile.open("../Tappa_10/resources/highscore.txt", std::ios::in);
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