#include "../../includes/game_elements/Ghost.hpp"
#include "../../includes/game_elements/GameState.hpp"
#include "../../includes/lib/textures.hpp"
#include <iostream>
#include <climits>
#include <algorithm>
#include <queue>
#include <array>
#include <unordered_map>

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
    sf::IntRect preferredAngle,
    std::string name,
    GameState &gameState,
    std::map<Direction, sf::Vector2i> GHOST_TEX_MAP) : map(nullptr),
                                                       dotLimit(dotLimit),
                                                       name(name),
                                                       gameState(gameState),
                                                       GHOST_TEX_MAP(GHOST_TEX_MAP),
                                                       preferredAngle(preferredAngle)
{
    setState(state);
    setScatterChasePattern();
    setSpeed();

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
    if (!gameState.pacman.isDead && !gameState.isWallBlinking)
    {
        if (scoreDisplayTimer > 0.f)
        {
            Ghost::drawScore();
        }
        else
        {
            stoppedForScore = false;
        }
    }

    if (stoppedForScore)
    {
        return;
    }

    float y = static_cast<float>((fPosition.x + position.x + 3.5f) * TILE_SIZE);
    float x = static_cast<float>((fPosition.y + position.y + 0.5f) * TILE_SIZE);

    sprite->setPosition({x, y});

    window.draw(*sprite);

    sf::RectangleShape rect(sf::Vector2f({TILE_SIZE, TILE_SIZE}));
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
    }

    // Disegna l'angolo preferito
    sf::RectangleShape preferredAngleRect(sf::Vector2f({(MAP_WIDTH * TILE_SIZE) / 2, (MAP_HEIGHT * TILE_SIZE) / 2}));
    preferredAngleRect.setFillColor(sf::Color::Transparent);
    preferredAngleRect.setOutlineThickness(1);
    if (name == "Blinky")
        preferredAngleRect.setOutlineColor(sf::Color::Red);
    else if (name == "Pinky")
        preferredAngleRect.setOutlineColor(sf::Color::Magenta);
    else if (name == "Inky")
        preferredAngleRect.setOutlineColor(sf::Color::Cyan);
    else if (name == "Clyde")
        preferredAngleRect.setOutlineColor(sf::Color(255, 165, 0)); // Orange
    else
        preferredAngleRect.setOutlineColor(sf::Color::White);

    preferredAngleRect.setPosition({preferredAngle.position.x * TILE_SIZE, (preferredAngle.position.y + 3) * TILE_SIZE});
    window.draw(preferredAngleRect);
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
    lastDirection = direction;
    direction = dir;
}

bool Ghost::isWall(int x, int y)
{
    if (!map)
        return false;

    if (x < 0 || x >= map->size() || y < 0 || y >= (*map)[0].size())
        return false;

    if (((*map)[x][y] == GHOST_DOOR_H || (*map)[x][y] == GHOST_DOOR_V))
    {
        if (state == CHASE)
            return true;
        if (state == SCARED && lastState == IN_HOUSE)
            return true;
        if (state == EATEN)
            return false;

        if (state == IN_HOUSE && isTransitioning)
            return false;
        else
            return true;
    }

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
        if (lastDirection != DOWN && !isWall(x - 1, y) && x - 1 >= 0 && x - 1 < map->size())
            possibleDirections.push_back(UP);
        if (lastDirection != UP && !isWall(x + 1, y) && x + 1 >= 0 && x + 1 < map->size())
            possibleDirections.push_back(DOWN);

        if (possibleDirections.empty())
        {
            if (lastDirection == UP && !isWall(x + 1, y) && x + 1 < map->size())
                possibleDirections.push_back(DOWN);
            else if (lastDirection == DOWN && !isWall(x - 1, y) && x - 1 >= 0)
                possibleDirections.push_back(UP);
        }
    }
    else
    {
        Direction randomDir = static_cast<Direction>(rand() % 4);
        if (!isWall(x + (randomDir == DOWN ? 1 : (randomDir == UP ? -1 : 0)), y + (randomDir == RIGHT ? 1 : (randomDir == LEFT ? -1 : 0))))
        {
            setDirection(randomDir);
            return;
        }
        else
        {
            for (Direction dir : {UP, RIGHT, DOWN, LEFT})
            {
                if (dir != randomDir && !isWall(x + (dir == DOWN ? 1 : (dir == UP ? -1 : 0)), y + (dir == RIGHT ? 1 : (dir == LEFT ? -1 : 0))))
                {
                    setDirection(dir);
                    return;
                }
            }
            setDirection(NONE);
        }
    }

    if (!possibleDirections.empty())
    {
        setDirection(possibleDirections[rand() % possibleDirections.size()]);
    }
}

sf::Vector2i Ghost::findFirstValidTile(sf::Vector2i start, sf::Vector2f dimensions)
{
    for (int y = start.y; y < dimensions.y; y++)
    {
        for (int x = start.x; x < dimensions.x; x++)
        {
            if (!isWall(x, y))
            {
                return {x, y};
            }
        }
    }
    return {-1, -1};
}

// https://medium.com/@RobuRishabh/classic-graph-algorithms-c-9773f2841f2e
void Ghost::findPathBFS(sf::Vector2i destination)
{
    if (!map)
        return;

    path.clear();

    targetTile = destination;
    if (isWall(targetTile.x, targetTile.y) && state == SCATTER)
    {
        targetTile = findFirstValidTile(destination, sf::Vector2f(MAP_WIDTH, MAP_HEIGHT));
    }

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

        if (current == targetTile)
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

void Ghost::eatenState(float elapsed)
{
    if (position != nearestExitTile)
    {
        if (!path.empty())
        {
            sf::Vector2i nextTile = path.back();
            computeNextDirection(nextTile);

            if (position == nextTile)
            {
                path.pop_back();
            }
        }
        else
        {
            computeNextDirection(nearestExitTile);
        }
        isTransitioning = true;
    }
    else
    {
        timeToEnterHouse += elapsed;
        setDirection(lastDirection);
        if (!enteredHouse)
            enteredHouse = true;

        if (timeToEnterHouse >= .5f)
        {
            setState(IN_HOUSE);
            path.clear();
            enteredHouse = true;
        }
    }
}

void Ghost::scatterState()
{
    bool isInScatterArea = preferredAngle.contains({position.y, position.x});

    if (!isInScatterArea)
    {
        if (path.empty())
        {
            findPathBFS({preferredAngle.position.y, preferredAngle.position.x});
        }
    }
    else
        path.clear();
}

void Ghost::behaviour()
{
    // Implement ghost behavior logic here
}

bool Ghost::isAlignedToCell()
{
    float tolerance = 0.075f;
    return std::abs(fPosition.x) < tolerance && std::abs(fPosition.y) < tolerance;
}

void Ghost::exitHouse()
{
    if (position != nearestExitTile)
    {
        if (!path.empty())
        {
            sf::Vector2i nextTile = path.back();
            computeNextDirection(nextTile);

            if (position == nextTile)
            {
                path.pop_back();
            }
        }
        else
        {
            computeNextDirection(nearestExitTile);
        }

        isTransitioning = true;
        enteredHouse = false;
    }
    else
    {
        setState(CHASE);
        isTransitioning = false;
        enteredHouse = false;
        path.clear();
    }
}

void Ghost::setScatterChasePattern()
{
    if (gameState.level < 5)
    {
        chaseScatterPattern[0] = 7.f; // Scatter
        chaseScatterPattern[2] = 7.f; // Scatter
        chaseScatterPattern[4] = 5.f; // Scatter
        if (gameState.level > 1)
        {
            chaseScatterPattern[5] = 1033.f;   // Chase
            chaseScatterPattern[6] = 1 / 60.f; // Scatter
        }
        else
        {
            chaseScatterPattern[5] = 20.f; // Chase
            chaseScatterPattern[6] = 5.f;  // Scatter
        }
    }
    else
    {
        chaseScatterPattern[0] = 5.f;
        chaseScatterPattern[2] = 5.f;
        chaseScatterPattern[5] = 1037.f;   // Chase
        chaseScatterPattern[6] = 1 / 60.f; // Scatter
    }

    chaseScatterPattern[1] = 20.f;   // Chase
    chaseScatterPattern[3] = 20.f;   // Chase
    chaseScatterPattern[7] = 9999.f; // Indefinite Chase
}

void Ghost::update(float elapsed)
{
    if (state == EATEN)
    {
        if (scoreDisplayTimer > 0.f)
        {
            scoreDisplayTimer -= elapsed;
        }
    }
    else if (state != IN_HOUSE && state != SCARED)
    {
        if (chaseScatterIndex % 2 == 0)
        {
            // Scatter
            if (chaseScatterPattern[chaseScatterIndex] < 0.f)
            {
                if (state != CHASE)
                {
                    setState(CHASE);
                }
                chaseScatterPattern[chaseScatterIndex] = 0.f;

                chaseScatterIndex++;
            }
            else
            {
                chaseScatterPattern[chaseScatterIndex] -= elapsed;
            }
        }
        else
        {
            // Chase
            if (chaseScatterIndex == 7)
                return;

            if (chaseScatterPattern[chaseScatterIndex] < 0.f)
            {
                if (state != SCATTER)
                {
                    setState(SCATTER);
                    findPathBFS({preferredAngle.position.y, preferredAngle.position.x});
                }
                chaseScatterPattern[chaseScatterIndex] = 0.f;

                chaseScatterIndex++;
            }
            else
            {
                chaseScatterPattern[chaseScatterIndex] -= elapsed;
            }
        }
    }

    move(elapsed);
    Ghost::animate(elapsed);
}

bool Ghost::isAtIntersection()
{
    int possibleDirections = 0;
    int x = position.x;
    int y = position.y;

    if (getOppositeDirection(direction) != UP && !isWall(x - 1, y))
    {
        possibleDirections++;
    }
    if (getOppositeDirection(direction) != DOWN && !isWall(x + 1, y))
    {
        possibleDirections++;
    }
    if (getOppositeDirection(direction) != LEFT && !isWall(x, y - 1))
    {
        possibleDirections++;
    }
    if (getOppositeDirection(direction) != RIGHT && !isWall(x, y + 1))
    {
        possibleDirections++;
    }

    return possibleDirections > 1;
}

void Ghost::move(float elapsed)
{
    if (!map || stoppedForScore || this == nullptr)
    {
        return;
    }

    if (state == EATEN)
    {
        Ghost::eatenState(elapsed);
    }
    else if (state == SCATTER)
    {
        Ghost::scatterState();
    }
    else if (state != SCARED)
    {
        behaviour();
    }

    if (!path.empty())
    {
        sf::Vector2i nextTile = path.back();
        computeNextDirection(nextTile);

        if (position == nextTile)
        {
            path.pop_back();
        }
    }
    else if (isAlignedToCell())
    {
        if (isAtIntersection())
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
    else
    {
        if (isAlignedToCell())
        {
            chooseDirection();
        }
    }

    if (std::abs(fPosition.x) >= 1.0f)
    {
        int moveTiles = static_cast<int>(std::round(fPosition.x));
        position.x += moveTiles;
        fPosition.x -= moveTiles;

        if (isTransitioning && position != nearestExitTile)
        {
            isTransitioning = false;
        }
    }
    if (std::abs(fPosition.y) >= 1.0f)
    {
        int moveTiles = static_cast<int>(std::round(fPosition.y));
        position.y += moveTiles;
        fPosition.y -= moveTiles;

        if (isTransitioning && position != nearestExitTile)
        {
            isTransitioning = false;
        }
    }

    if (std::abs(fPosition.x) < 0.01f)
    {
        fPosition.x = 0;
    }

    if (std::abs(fPosition.y) < 0.01f)
    {
        fPosition.y = 0;
    }

    if (state != EATEN)
    {
        eat(position.x, position.y);
    }
}

void Ghost::animate(float elapsed)
{
    if (state == EATEN)
    {
        auto eyeTexPos = GHOST_EYES_TEX_MAP.find(direction);
        if (eyeTexPos == GHOST_EYES_TEX_MAP.end())
        {
            setDirection(LEFT);
            eyeTexPos = GHOST_EYES_TEX_MAP.find(direction);
        }
        sprite->setTextureRect(sf::IntRect({eyeTexPos->second.x * TILE_SIZE / 2, GHOST_EYES_TEX_MAP.at(direction).y * TILE_SIZE / 2}, {TILE_SIZE / 2, TILE_SIZE / 2}));
    }
    else if (state != SCARED)
    {
        if (GHOST_ANIM_MAP.find(direction) != GHOST_ANIM_MAP.end())
        {
            std::map<Direction, Animation>::iterator it = GHOST_ANIM_MAP.find(direction);
            it->second.update(elapsed);
        }
        else
        {
            setDirection(LEFT);
        }
    }
    else
    {
        if (gameState.pacman.powerPelletDurationTimer <= gameState.pacman.powerPelletDuration * .4f)
        {
            if (blinkingTime > 0.f)
            {
                blinkingTime -= elapsed;
                if (isWhite)
                {
                    backScaredAnim->update(elapsed);
                }
                else
                {
                    scaredAnim->update(elapsed);
                }
            }
            else
            {
                blinkingTime = 0.2f;
                isWhite = !isWhite;
            }
        }
        else
        {
            scaredAnim->update(elapsed);
        }
    }
}

/*void Ghost::move(float elapsed)
{
    if (!map || stoppedForScore || this == nullptr)
    {
        return;
    }

    if (state == EATEN)
    {
        auto eyeTexPos = GHOST_EYES_TEX_MAP.find(direction);
        if (eyeTexPos == GHOST_EYES_TEX_MAP.end())
        {
            setDirection(LEFT);
            eyeTexPos = GHOST_EYES_TEX_MAP.find(direction);
        }
        sprite->setTextureRect(sf::IntRect({eyeTexPos->second.x * TILE_SIZE / 2, GHOST_EYES_TEX_MAP.at(direction).y * TILE_SIZE / 2}, {TILE_SIZE / 2, TILE_SIZE / 2}));

        if (position != nearestExitTile)
        {
            if (!path.empty())
            {
                sf::Vector2i nextTile = path.back();
                computeNextDirection(nextTile);

                if (position == nextTile)
                {
                    path.pop_back();
                }
            }
            else
            {
                computeNextDirection(nearestExitTile);
            }
            isTransitioning = true;
        }
        else
        {
            timeToEnterHouse += elapsed;
            setDirection(lastDirection);
            if (!enteredHouse)
                enteredHouse = true;

            if (timeToEnterHouse >= .5f)
            {
                setState(IN_HOUSE);
                path.clear();
                enteredHouse = true;
            }
        }
    }
    else
    {
        sf::Vector2i pacmanPosition = getPacmanPosition();
        if (distance(pacmanPosition) < 10.0f && state == NORMAL)
        {
            state = CHASE;
            isChasing = true;
        }
        else if (state == CHASE && distance(pacmanPosition) >= 10.0f)
        {
            isChasing = false;
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
                        computeNextDirection(nextTile);

                        if (position == nextTile)
                        {
                            path.pop_back();
                        }
                    }
                    else
                    {
                        computeNextDirection(nearestExitTile);
                    }

                    isTransitioning = true;
                    enteredHouse = false;
                }
                else
                {
                    setState(NORMAL);
                    isTransitioning = false;
                    enteredHouse = false;
                    path.clear();
                }
            }
            else
            {
                isTransitioning = false;
                enteredHouse = true;
            }
        }
        else if (state == NORMAL)
        {
            enteredHouse = false;
            isTransitioning = false;
        }
        else if (state == CHASE)
        {
            isTransitioning = false;
            if (path.empty() && isChasing)
            {
                findPathBFS(pacmanPosition);
            }

            if (!path.empty())
            {
                sf::Vector2i nextTile = path.back();
                computeNextDirection(nextTile);

                if (position == nextTile)
                {
                    path.pop_back();
                }
            }
            else
            {
                state = NORMAL;
                isChasing = false;
            }
        }
        else
        {
            isTransitioning = false;
            enteredHouse = false;
        }

        if (GHOST_ANIM_MAP.find(direction) != GHOST_ANIM_MAP.end())
        {
            std::map<Direction, Animation>::iterator it = GHOST_ANIM_MAP.find(direction);
            it->second.update(elapsed);
        }
        else
        {
            setDirection(LEFT);
        }
    }

    float tolerance = 0.01f;
    bool alignedToCell = std::abs(fPosition.x) < tolerance && std::abs(fPosition.y) < tolerance;

    if (alignedToCell && !isTransitioning && state != CHASE)
    {
        int possibleDirectionsCount = 0;

        // UP
        if (!isWall(position.x - 1, position.y))
        {
            possibleDirectionsCount++;
        }

        // DOWN
        if (!isWall(position.x + 1, position.y))
        {
            possibleDirectionsCount++;
        }

        // LEFT
        if (!isWall(position.x, position.y - 1))
        {
            possibleDirectionsCount++;
        }

        // RIGHT
        if (!isWall(position.x, position.y + 1))
        {
            possibleDirectionsCount++;
        }

        if (possibleDirectionsCount > 1)
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
    else
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
*/

// Calcola la distanza tra il fantasma e il bersaglio
double Ghost::distance(sf::Vector2i target)
{
    return std::sqrt(std::pow(target.x - position.x, 2) + std::pow(target.y - position.y, 2));
}

// Un semplice pathfinding in base alla posizione del fantasma e del bersaglio
void Ghost::computeNextDirection(sf::Vector2i destination)
{
    if (position.x < destination.x && !isWall(position.x + 1, position.y))
        setDirection(DOWN);
    else if (position.x > destination.x && !isWall(position.x - 1, position.y))
        setDirection(UP);
    else if (position.y < destination.y && !isWall(position.x, position.y + 1))
        setDirection(RIGHT);
    else if (position.y > destination.y && !isWall(position.x, position.y - 1))
        setDirection(LEFT);
}

void Ghost::setState(GhostState newState)
{
    if (state != newState)
    {
        lastState = state;
        state = newState;
    }

    if (state == SCATTER)
    {
        std::cout << name << ":" << "SCATTER" << std::endl;
    }
    else if (state == CHASE)
    {
        std::cout << name << ":" << "CHASE" << std::endl;
    }

    if (state == SCARED)
    {
        path.clear();
        setDirection(getOppositeDirection(direction));

        if (gameState.level == 1)
        {
            currentSpeed = speed * .5f;
        }
        else if (gameState.level >= 2 && gameState.level <= 4)
        {
            currentSpeed = speed * .55f;
        }
        else if (gameState.level >= 5 && gameState.level <= 20)
        {
            currentSpeed = speed * .6f;
        }
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
        std::cerr << "Nessuna tile di uscita disponibile" << std::endl;
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
    if (distance(getPacmanPosition()) < COLLIDE_BOX)
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
            gameState.pacman.isDead = true;
            gameState.lives--;
        }
    }
}

void Ghost::respawn()
{
    setState(IN_HOUSE);
    setPosition(spawn.x, spawn.y);
    setDirection(NONE);
    isTransitioning = false;
    getExitTile();
    findPathBFS(nearestExitTile);
    chaseScatterIndex = 0;
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

Direction Ghost::getPacmanDirection()
{
    return gameState.pacman.getDirection();
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

void Ghost::setSpeed()
{
    if (gameState.level == 1)
    {
        speed = GHOST_SPEED * .75f;
    }
    else if (gameState.level >= 2 && gameState.level <= 4)
    {
        speed = GHOST_SPEED * .85f;
    }
    else
    {
        speed = GHOST_SPEED * .95f;
    }

    currentSpeed = speed;
}