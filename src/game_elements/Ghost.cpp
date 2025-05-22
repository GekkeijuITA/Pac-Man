#include "../../includes/game_elements/Ghost.hpp"
#include "../../includes/game_elements/GameState.hpp"
#include "../../includes/lib/textures.hpp"
#include <iostream>
#include <climits>
#include <algorithm>
#include <queue>
#include <array>
#include <unordered_map>

#define COLLIDE_BOX 1.5f
#define SCORE_DISPLAY_TIME 1.5f;

namespace std
{
    template <>
    struct hash<sf::Vector2i>
    {
        size_t operator()(const sf::Vector2i &vector) const noexcept
        {
            size_t h1 = std::hash<int>{}(vector.x);
            size_t h2 = std::hash<int>{}(vector.y);
            return h1 ^ (h2 << 1);
        }
    };
}

namespace sf
{
    bool operator<(const sf::Vector2i a, const sf::Vector2i b)
    {
        if (a.x != b.x)
            return a.x < b.x;
        return a.y < b.y;
    }
}

Ghost::Ghost(
    GhostState state,
    int dotLimit,
    std::string name,
    GameState &gameState,
    std::map<Direction, sf::Vector2i> GHOST_TEX_MAP) : map(nullptr),
                                                       state(state),
                                                       dotLimit(dotLimit),
                                                       name(name),
                                                       gameState(gameState),
                                                       GHOST_TEX_MAP(GHOST_TEX_MAP)
{
    speed = 2.5f;
    currentSpeed = speed;
    direction = NONE;
    lastDirection = NONE;
    isTransitioning = false;
    timeToEnterHouse = 0.f;
    enteredHouse = false;
    lastState = state;
    score = 0;
    scoreDisplayTimer = SCORE_DISPLAY_TIME;
    stoppedForScore = false;

    if (!tex.loadFromFile(ASSET))
    {
        std::cerr << "Errore nel caricamento della texture del fantasma" << std::endl;
        exit(1);
    }

    if (GHOST_TEX_MAP.empty())
    {
        std::cerr << "Errore nel caricamento della texture del fantasma" << std::endl;
        exit(1);
    }

    sprite = std::make_unique<sf::Sprite>(createSprite(tex, GHOST_TEX_MAP.at(Direction::LEFT), {2.f, 2.f}, 1.5f, TILE_SIZE / 2, true));

    for (Direction dir : {UP, DOWN, LEFT, RIGHT, NONE})
    {
        Animation::insertAnimation(dir, GHOST_ANIM_MAP, GHOST_TEX_MAP, *sprite, 1, 0.2f);
    }

    scaredAnim = std::make_unique<Animation>(*sprite);
    scaredAnim->addFrame({sf::IntRect({(GHOST_SCARED.x + 1) * (TILE_SIZE / 2), (GHOST_SCARED.y) * (TILE_SIZE / 2)}, {TILE_SIZE / 2, TILE_SIZE / 2}), frameDuration});
    scaredAnim->addFrame({sf::IntRect({(GHOST_SCARED.x) * (TILE_SIZE / 2), (GHOST_SCARED.y) * (TILE_SIZE / 2)}, {TILE_SIZE / 2, TILE_SIZE / 2}), frameDuration});

    backScaredAnim = std::make_unique<Animation>(*sprite);
    backScaredAnim->addFrame({sf::IntRect({(GHOST_SCARED.x + 3) * (TILE_SIZE / 2), (GHOST_SCARED.y) * (TILE_SIZE / 2)}, {TILE_SIZE / 2, TILE_SIZE / 2}), frameDuration});
    backScaredAnim->addFrame({sf::IntRect({(GHOST_SCARED.x + 2) * (TILE_SIZE / 2), (GHOST_SCARED.y) * (TILE_SIZE / 2)}, {TILE_SIZE / 2, TILE_SIZE / 2}), frameDuration});
}

void Ghost::draw(sf::RenderWindow &window)
{
    if (stoppedForScore)
    {
        return;
    }

    /*sf::Vector2i texPos;
    if (state == EATEN)
    {
        texPos = GHOST_EYES_TEX_MAP.at(direction);
    }
    else if (state == SCARED)
    {
        texPos = GHOST_SCARED;
    }
    else
    {

    }*/

    float y = static_cast<float>((fPosition.x + position.x + 3.5f) * TILE_SIZE);
    float x = static_cast<float>((fPosition.y + position.y + 0.5f) * TILE_SIZE);

    sprite->setPosition({x, y});

    window.draw(*sprite);

    /*sf::RectangleShape rect(sf::Vector2f({TILE_SIZE, TILE_SIZE}));
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineThickness(1);
    // Disegna la BFS
    for (const auto &p : path)
    {
        float y_l = static_cast<float>((p.x + 3) * TILE_SIZE);
        float x_l = static_cast<float>(p.y * TILE_SIZE);
        if (name == "Blinky")
            rect.setOutlineColor(sf::Color::Red);
        else if (name == "Pinky")
            rect.setOutlineColor(sf::Color::Magenta);
        else if (name == "Inky")
            rect.setOutlineColor(sf::Color::Cyan);
        else if (name == "Clyde")
            rect.setOutlineColor(sf::Color(255, 165, 0)); // Orange
        else
            rect.setOutlineColor(sf::Color::White);

        rect.setPosition({x_l, y_l});
        window.draw(rect);
    }*/
}

void Ghost::setPosition(int x, int y)
{
    position.x = x;
    position.y = y;
    fPosition = {0.f, 0.f};
}

void Ghost::setMap(std::vector<std::vector<char>> *newMap)
{
    map = newMap;
    getExitTile();
    findPathBFS(nearestExitTile);
}

void Ghost::setDirection(Direction dir)
{
    direction = dir;
}

bool Ghost::isWall(int x, int y)
{
    if (!map)
        return false;

    if (state == NORMAL && (*map)[x][y] == GHOST_DOOR_H)
        return true;

    return (*map)[x][y] == LINE_H || (*map)[x][y] == LINE_V || (*map)[x][y] == CORNER_0 || (*map)[x][y] == CORNER_90 || (*map)[x][y] == CORNER_180 || (*map)[x][y] == CORNER_270;
}

void Ghost::chooseDirection()
{
    std::vector<Direction> possibleDirections;
    int x = position.x;
    int y = position.y;

    bool isOrWasInHouse = (state == SCARED && lastState == IN_HOUSE) || state == IN_HOUSE;

    if (isOrWasInHouse)
    {
        if (lastDirection != DOWN && !isWall(x - 1, y))
            possibleDirections.push_back(UP);
        if (lastDirection != UP && !isWall(x + 1, y))
            possibleDirections.push_back(DOWN);

        if (possibleDirections.empty())
        {
            if (lastDirection == UP && !isWall(x + 1, y))
                possibleDirections.push_back(DOWN);
            else if (lastDirection == DOWN && !isWall(x - 1, y))
                possibleDirections.push_back(UP);
        }
    }
    else
    {
        if (lastDirection != DOWN && !isWall(x - 1, y))
            possibleDirections.push_back(UP);
        if (lastDirection != UP && !isWall(x + 1, y))
            possibleDirections.push_back(DOWN);
        if (lastDirection != RIGHT && !isWall(x, y - 1))
            possibleDirections.push_back(LEFT);
        if (lastDirection != LEFT && !isWall(x, y + 1))
            possibleDirections.push_back(RIGHT);

        if (possibleDirections.empty())
        {
            if (!isWall(x - 1, y))
                possibleDirections.push_back(UP);
            if (!isWall(x + 1, y))
                possibleDirections.push_back(DOWN);
            if (!isWall(x, y - 1))
                possibleDirections.push_back(LEFT);
            if (!isWall(x, y + 1))
                possibleDirections.push_back(RIGHT);
        }
    }
    if (!possibleDirections.empty())
    {
        direction = possibleDirections[rand() % possibleDirections.size()];
        lastDirection = direction;
    }
}

// https://medium.com/@RobuRishabh/classic-graph-algorithms-c-9773f2841f2e
void Ghost::findPathBFS(sf::Vector2i destination)
{
    if (!map)
        return;

    path.clear();

    std::unordered_map<sf::Vector2i, bool> visited;
    std::unordered_map<sf::Vector2i, sf::Vector2i> parent;
    std::queue<sf::Vector2i> q;
    visited[position] = true;
    parent[position] = position;
    q.push(position);

    while (!q.empty())
    {
        sf::Vector2i current = q.front();
        q.pop();

        if (current == destination)
        {
            sf::Vector2i step = current;
            while (step != position)
            {
                path.push_back(step);
                step = parent[step];
            }
            path.push_back(position);

            return;
        }

        std::vector<sf::Vector2i> neighbours = {
            {current.x - 1, current.y},  // UP
            {current.x + 1, current.y},  // DOWN
            {current.x, current.y - 1},  // LEFT
            {current.x, current.y + 1}}; // RIGHT

        for (sf::Vector2i neighbour : neighbours)
        {
            if (!visited[neighbour] &&
                neighbour.x >= 0 &&
                neighbour.x < map->size() &&
                neighbour.y >= 0 &&
                neighbour.y < (*map)[0].size() &&
                !isWall(neighbour.x, neighbour.y))
            {
                visited[neighbour] = true;
                parent[neighbour] = current;
                q.push(neighbour);
            }
        }
    }
}

void Ghost::move(float elapsed)
{
    if (stoppedForScore)
    {
        return;
    }

    if (state == EATEN)
    {
        sprite->setTextureRect(sf::IntRect({GHOST_EYES_TEX_MAP.at(direction).x * TILE_SIZE / 2, GHOST_EYES_TEX_MAP.at(direction).y * TILE_SIZE / 2}, {TILE_SIZE / 2, TILE_SIZE / 2}));
    }
    else
    {
        std::map<Direction, Animation>::iterator it = GHOST_ANIM_MAP.find(direction);
        if (it != GHOST_ANIM_MAP.end())
            it->second.update(elapsed);
    }

    if (state == IN_HOUSE)
    {
        if (gameState.pacman.getDotEaten() >= dotLimit)
        {
            if (position != nearestExitTile)
            {
                if (!path.empty())
                {
                    sf::Vector2i nextTile = path.back();

                    if (position == nextTile)
                    {
                        path.pop_back();
                    }
                    computeNextDirection(nextTile);
                }
                else
                {
                    computeNextDirection(nearestExitTile);
                }
            }
            else
            {
                setState(NORMAL);
                isTransitioning = true;
            }
        }
        else
        {
            chooseDirection();
        }
    }
    else if (state == EATEN)
    {
        if (position != nearestExitTile && !enteredHouse)
        {
            if (!path.empty())
            {
                sf::Vector2i nextTile = path.back();

                if (position == nextTile)
                {
                    path.pop_back();
                }
                computeNextDirection(nextTile);
            }
            else
            {
                computeNextDirection(nearestExitTile);
            }
            timeToEnterHouse = 0.f;
        }
        else
        {
            timeToEnterHouse += elapsed;
            direction = lastDirection;
            if (!enteredHouse)
                enteredHouse = true;

            if (timeToEnterHouse >= .5f)
            {
                setState(IN_HOUSE);
                isTransitioning = true;
                path.clear();
                enteredHouse = false;
            }
        }
    }

    float tolerance = 0.01f;
    bool alignedToCell = std::abs(fPosition.x) < tolerance && std::abs(fPosition.y) < tolerance;

    if (alignedToCell && !isTransitioning)
    {
        bool canMove = false;
        switch (direction)
        {
        case UP:
            canMove = !isWall(position.x - 1, position.y);
            break;
        case DOWN:
            canMove = !isWall(position.x + 1, position.y);
            break;
        case LEFT:
            canMove = !isWall(position.x, position.y - 1);
            break;
        case RIGHT:
            canMove = !isWall(position.x, position.y + 1);
            break;
        }

        if (!canMove)
        {
            chooseDirection();
        }
    }

    sf::Vector2i nextTile = position;

    switch (direction)
    {
    case UP:
        nextTile.x--;
        break;
    case DOWN:
        nextTile.x++;
        break;
    case LEFT:
        nextTile.y--;
        break;
    case RIGHT:
        nextTile.y++;
        break;
    default:
        break;
    }

    if (!isWall(nextTile.x, nextTile.y))
    {
        sf::Vector2f movement(0, 0);
        switch (direction)
        {
        case UP:
            movement.x = -currentSpeed * elapsed;
            break;
        case DOWN:
            movement.x = currentSpeed * elapsed;
            break;
        case LEFT:
            movement.y = -currentSpeed * elapsed;
            break;
        case RIGHT:
            movement.y = currentSpeed * elapsed;
            break;
        default:
            break;
        }
        fPosition += movement;
    }
    else if (!isTransitioning)
    {
        chooseDirection();
    }

    if (std::abs(fPosition.x) >= 1.0f)
    {
        position.x += static_cast<int>(fPosition.x);
        fPosition.x -= static_cast<int>(fPosition.x);

        if (isTransitioning && position != nearestExitTile)
        {
            isTransitioning = false;
        }
    }
    if (std::abs(fPosition.y) >= 1.0f)
    {
        position.y += static_cast<int>(fPosition.y);
        fPosition.y -= static_cast<int>(fPosition.y);

        if (isTransitioning && position != nearestExitTile)
        {
            isTransitioning = false;
        }
    }

    if (state != EATEN)
    {
        eat(position.x, position.y);
    }
}

// Calcola la distanza tra il fantasma e il bersaglio
double Ghost::distance(sf::Vector2i target)
{
    return std::sqrt(std::pow(target.x - position.x, 2) + std::pow(target.y - position.y, 2));
}

// Un semplice pathfinding in base alla posizione del fantasma e del bersaglio
void Ghost::computeNextDirection(sf::Vector2i destination)
{
    if (position.x < destination.x && !isWall(position.x + 1, position.y))
        direction = DOWN;
    else if (position.x > destination.x && !isWall(position.x - 1, position.y))
        direction = UP;
    else if (position.y < destination.y && !isWall(position.x, position.y + 1))
        direction = RIGHT;
    else if (position.y > destination.y && !isWall(position.x, position.y - 1))
        direction = LEFT;

    lastDirection = direction;
}

void Ghost::setState(GhostState newState)
{
    if (state != newState)
    {
        lastState = state;
        state = newState;
    }

    if (state == SCARED)
    {
        direction = getOppositeDirection(direction);
        lastDirection = direction;

        currentSpeed = speed / 1.5f;
    }
    else if (state == EATEN)
    {
        currentSpeed = speed * 2.f;
    }
    else
    {
        currentSpeed = speed;
    }
}

void Ghost::addExitTile(int x, int y)
{
    exitTiles.push_back({x, y});
}

void Ghost::getExitTile()
{
    if (exitTiles.empty())
    {
        std::cerr << "Nessun tile di uscita disponibile" << std::endl;
        return;
    }

    std::map<sf::Vector2i, int> distances;

    for (const auto &tile : exitTiles)
    {
        int dist = std::abs(position.x - tile.x) + std::abs(position.y - tile.y);
        distances[tile] = dist;
    }

    auto minIt = std::min_element(distances.begin(), distances.end(), [](const auto &a, const auto &b)
                                  { return a.second < b.second; });
    nearestExitTile = minIt->first;
}

void Ghost::eat(int x, int y)
{
    if (position == gameState.pacman.getPosition())
    {
        if (state == SCARED)
        {
            setState(EATEN);
            score = 200 * (std::pow(2, gameState.pacman.ghostStreak));

            if (gameState.pacman.ghostStreak < 4)
                gameState.pacman.ghostStreak++;
            else
                gameState.pacman.ghostStreak = 0;

            stoppedForScore = true;
            scoreDisplayTimer = SCORE_DISPLAY_TIME;
            gameState.score += score;
            getExitTile();
            findPathBFS(nearestExitTile);
        }
        else
        {
            gameState.lives--;
            gameState.resetRound();
        }
    }
}

void Ghost::respawn(GhostState state)
{
    setState(state);
    setPosition(spawn.x, spawn.y);
    setDirection(NONE);
    lastDirection = NONE;
    isTransitioning = false;
    getExitTile();
    findPathBFS(nearestExitTile);
}

void Ghost::drawScore()
{
    sf::Vector2i scorePos;

    switch (score)
    {
    case 200:
        scorePos = GHOST_SCORE_200;
        break;
    case 400:
        scorePos = GHOST_SCORE_400;
        break;
    case 800:
        scorePos = GHOST_SCORE_800;
        break;
    case 1600:
        scorePos = GHOST_SCORE_1600;
        break;
    default:
        return;
    }

    sf::Sprite sprite = createSprite(tex, scorePos, {2.f, 2.f}, 1.5f, TILE_SIZE / 2, true);
    sprite.setPosition({(fPosition.y + position.y + .5f) * TILE_SIZE, (fPosition.x + position.x + 3.5f) * TILE_SIZE});
    gameState.window.draw(sprite);
}

sf::Vector2i Ghost::getPacmanPosition()
{
    return gameState.pacman.getPosition();
}

Direction Ghost::getOppositeDirection(Direction dir)
{
    switch (dir)
    {
    case UP:
        return DOWN;
    case DOWN:
        return UP;
    case LEFT:
        return RIGHT;
    case RIGHT:
        return LEFT;
    default:
        return NONE;
    }
}
