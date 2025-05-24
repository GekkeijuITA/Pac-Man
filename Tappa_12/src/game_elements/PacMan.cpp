#include "../../includes/game_elements/PacMan.hpp"
#include "../../includes/game_elements/GameState.hpp"
#include "../../includes/lib/textures.hpp"
#include "../../includes/lib/TileFactory.hpp"
#include <iostream>

PacMan::PacMan(GameState &gameState) : map(nullptr), gameState(gameState)
{
    speed = 5.f;
    dotEaten = 0;
    direction = LEFT;
    nextDirection = NONE;
    powerPellet = false;
    powerPelletDuration = POWER_PELLET_DURATION; // in secondi
    ghostStreak = 0;

    if (!tex.loadFromFile(ASSET))
    {
        std::cerr << "Errore nel caricamento della texture di PacMan" << std::endl;
        exit(1);
    }

    sprite = std::make_unique<sf::Sprite>(createSprite(tex, LEFT_PACMAN, {2.f, 2.f}, 1.5f, TILE_SIZE / 2, true));

    float frameDuration = 0.07f;
    for (Direction dir : {UP, DOWN, LEFT, RIGHT, NONE})
    {
        Animation::insertAnimation(dir, PACMAN_ANIM_MAP, PACMAN_TEX_MAP, *sprite, -1, frameDuration);
        std::map<Direction, Animation>::iterator it = PACMAN_ANIM_MAP.find(dir);
        if (it != PACMAN_ANIM_MAP.end())
        {
            it->second.addFrame({sf::IntRect(
                                     {(PACMAN_FULL.x) * (TILE_SIZE / 2), (PACMAN_FULL.y) * (TILE_SIZE / 2)},
                                     {(TILE_SIZE / 2), (TILE_SIZE / 2)}),
                                 frameDuration});
        }
    }

    setDeathAnimation();
}

void PacMan::draw(sf::RenderWindow &window)
{
    float y = static_cast<float>((fPosition.x + position.x + 3 + 0.5f) * TILE_SIZE);
    float x = static_cast<float>((fPosition.y + position.y + 0.5f) * TILE_SIZE);

    sprite->setPosition({x, y});

    window.draw(*sprite);
}

void PacMan::setPosition(int x, int y)
{
    position.x = x;
    position.y = y;
    fPosition = {0.f, 0.f};
}

void PacMan::setNextDirection(Direction dir)
{
    nextDirection = dir;
}

void PacMan::setMap(std::vector<std::vector<char>> *newMap)
{
    map = newMap;
}

bool PacMan::isWall(int x, int y)
{
    if (!map)
        return false;

    return (*map)[x][y] == LINE_H || (*map)[x][y] == LINE_V || (*map)[x][y] == CORNER_0 || (*map)[x][y] == CORNER_90 || (*map)[x][y] == CORNER_180 || (*map)[x][y] == CORNER_270 || (*map)[x][y] == GHOST_DOOR_H;
}

void PacMan::updateDirection()
{
    if (nextDirection != NONE && nextDirection != direction)
    {
        sf::Vector2i dirVec(0, 0);
        switch (nextDirection)
        {
        case UP:
            dirVec.x = -1;
            break;
        case DOWN:
            dirVec.x = 1;
            break;
        case LEFT:
            dirVec.y = -1;
            break;
        case RIGHT:
            dirVec.y = 1;
            break;
        default:
            break;
        }

        float alignmentThreshold = 0.2f;

        if (!isWall(position.x + dirVec.x, position.y + dirVec.y) &&
            std::abs(fPosition.x) < alignmentThreshold &&
            std::abs(fPosition.y) < alignmentThreshold)
        {
            direction = nextDirection;
            fPosition = {0.f, 0.f};
            setNextDirection(NONE);
        }
    }
}

void PacMan::move(float elapsed)
{
    std::map<Direction, Animation>::iterator it = PACMAN_ANIM_MAP.find(direction);
    it->second.update(elapsed);

    sf::Vector2f new_fPosition = fPosition;

    switch (direction)
    {
    case UP:
        new_fPosition.x -= speed * elapsed;
        break;
    case DOWN:
        new_fPosition.x += speed * elapsed;
        break;
    case LEFT:
        new_fPosition.y -= speed * elapsed;
        break;
    case RIGHT:
        new_fPosition.y += speed * elapsed;
        break;
    default:
        break;
    }

    sf::Vector2i newPosition = position + static_cast<sf::Vector2i>(new_fPosition);

    if (!isWall(newPosition.x, newPosition.y))
    {
        fPosition = new_fPosition;
    }

    if (std::abs(fPosition.x) >= 1.0f)
    {
        int move_x = static_cast<int>(fPosition.x);
        position.x += move_x;
        fPosition.x -= move_x;
    }

    if (std::abs(fPosition.y) >= 1.0f)
    {
        int move_y = static_cast<int>(fPosition.y);
        position.y += move_y;
        fPosition.y -= move_y;
    }

    eat(position.x, position.y);
}

void PacMan::eat(int x, int y)
{
    if (!map || x < 0 || x >= map->size() || y < 0 || y >= (*map)[0].size())
        return;

    switch ((*map)[x][y])
    {
    case PACDOT:
        (*map)[x][y] = EMPTY_BLOCK;
        dotEaten++;
        gameState.score += 10;
        std::cout << "Pac-Man ate: " << dotEaten << " dots total dots to eat to win: " << gameState.eatableTiles << std::endl;
        break;
    case POWERPELLET:
        (*map)[x][y] = EMPTY_BLOCK;
        if (powerPellet)
            break;
        powerPellet = true;
        gameState.score += 50;
        powerPelletDuration = POWER_PELLET_DURATION;
        ghostStreak = 0;
        dotEaten++;
        std::cout << "Pac-Man ate: " << dotEaten << " dots total dots to eat to win: " << gameState.eatableTiles << std::endl;

        if (gameState.blinky.state != Ghost::EATEN)
            gameState.blinky.setState(Ghost::SCARED);
        if (gameState.pinky.state != Ghost::EATEN)
            gameState.pinky.setState(Ghost::SCARED);
        if (gameState.inky.state != Ghost::EATEN)
            gameState.inky.setState(Ghost::SCARED);
        if (gameState.clyde.state != Ghost::EATEN)
            gameState.clyde.setState(Ghost::SCARED);
    default:
        break;
    }
}

int PacMan::getDotEaten()
{
    return dotEaten;
}

void PacMan::respawn()
{
    setPosition(spawn.x, spawn.y);
    setNextDirection(NONE);
    direction = LEFT;
    powerPellet = false;
    powerPelletDuration = POWER_PELLET_DURATION;
    setDeathAnimation();

    sprite->setTexture(tex);
    if (!PACMAN_ANIM_MAP.empty())
    {
        std::map<Direction, Animation>::iterator it = PACMAN_ANIM_MAP.find(direction);
        if (it != PACMAN_ANIM_MAP.end())
        {
            it->second.reset();
            sprite->setTextureRect(it->second.frames[0].rect);
        }
    }
}

sf::Vector2i PacMan::getPosition()
{
    return position;
}

void PacMan::setDeathAnimation()
{
    Animation::insertAnimation(DEATH_ANIMATION, deathFrames, *sprite, .2f, false);
}