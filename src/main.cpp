#include "../includes/textures.hpp"
#include "../includes/global_values.hpp"
#include "../includes/State.hpp"
#include "../includes/text_ui.hpp"

#include <fstream>
#include <iostream>

State::State(unsigned w, unsigned h, std::string title) : lives(3), score(0), highscore(0), pacman()
{
    recentFruits.clear();

    float mapRatio = (float)(MAP_WIDTH) / (MAP_HEIGHT + 5);
    float screenRatio = (float)w / h;

    float scaleFactor = 0.7f;
    unsigned int windowWidth, windowHeight;

    if (mapRatio > screenRatio)
    {
        windowWidth = w * scaleFactor;
        windowHeight = windowWidth / mapRatio;
    }
    else
    {
        windowHeight = h * scaleFactor;
        windowWidth = windowHeight * mapRatio;
    }

    window = sf::RenderWindow(sf::VideoMode({windowWidth, windowHeight}), title);
    window.setPosition({static_cast<int>((w - windowWidth) / 2),
                        static_cast<int>((h - windowHeight) / 2)});
    window.setFramerateLimit(60);

    srand(time(0));
    blinky = std::make_unique<Blinky>(pacman, windowWidth, windowHeight);
    pinky = std::make_unique<Pinky>(pacman, windowWidth, windowHeight);
    inky = std::make_unique<Inky>(pacman, windowWidth, windowHeight);
    clyde = std::make_unique<Clyde>(pacman, windowWidth, windowHeight);

    sf::View view;
    view.setSize({MAP_WIDTH * TILE_SIZE, (MAP_HEIGHT + 5) * TILE_SIZE});
    view.setCenter({view.getSize().x / 2.f, view.getSize().y / 2.f});
    window.setView(view);

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
}

void State::update(float elapsed)
{
    collisions(elapsed);
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

void State::bounds()
{
    checkBounds(pacman.position);
    checkBounds(blinky->position);
    checkBounds(pinky->position);
    checkBounds(inky->position);
    checkBounds(clyde->position);
}

void State::ghost_collisions(float elapsed)
{
}

void State::collisions(float elapsed)
{
    int next_x = pacman.position.x;
    int next_y = pacman.position.y;

    pacman.updateDirection();

    switch (pacman.direction)
    {
    case PacMan::UP:
        next_x--;
        break;
    case PacMan::DOWN:
        next_x++;
        break;
    case PacMan::LEFT:
        next_y--;
        break;
    case PacMan::RIGHT:
        next_y++;
        break;
    default:
        break;
    }

    if (!pacman.isWall(next_x, next_y))
    {
        pacman.move(elapsed);
    }

    blinky->move(elapsed);
    pinky->move(elapsed);
    inky->move(elapsed);
    clyde->move(elapsed);

    bounds();
}

bool State::getMap(std::string mapPath)
{
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
                map.back()[i] = EMPTY_BLOCK;
            }
            else if (row[i] == BLINKY)
            {
                blinky->setPosition(map.size() - 1, i);
                map.back()[i] = EMPTY_BLOCK;
            }
            else if (row[i] == PINKY)
            {
                pinky->setPosition(map.size() - 1, i);
                map.back()[i] = EMPTY_BLOCK;
            }
            else if (row[i] == INKY)
            {
                inky->setPosition(map.size() - 1, i);
                map.back()[i] = EMPTY_BLOCK;
            }
            else if (row[i] == CLYDE)
            {
                clyde->setPosition(map.size() - 1, i);
                map.back()[i] = EMPTY_BLOCK;
            }
            else if (row[i] == GHOST_DOOR)
            {
                blinky->addExitTile(map.size() - 1, i);
                pinky->addExitTile(map.size() - 1, i);
                inky->addExitTile(map.size() - 1, i);
                clyde->addExitTile(map.size() - 1, i);
            }
        }
    }
    mapFile.close();

    pacman.setMap(&map);

    blinky->setMap(&map);
    pinky->setMap(&map);
    inky->setMap(&map);
    clyde->setMap(&map);

    return true;
}

void State::doGraphics()
{
    for (int r = 0; r < MAP_HEIGHT; r++)
    {
        float y = (r + 3) * TILE_SIZE; // Lasciamo le prime due righe per il punteggi
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
            case CHERRY:
            {
                drawFruit(c + 0.5f, r + 3.5f, {2, 3}, 1.5f);
                break;
            }
            default:
                break;
            }
        }
    }

    blinky->draw(window);
    pinky->draw(window);
    inky->draw(window);
    clyde->draw(window);
    pacman.draw(window);

    /*sf::Color gridColor = sf::Color(255, 255, 255, 100); // Colore grigio semi-trasparente
    float thickness = 1.0f;

    for (int x = 0; x <= MAP_WIDTH; x++)
    {
        sf::RectangleShape line(sf::Vector2f({thickness, (MAP_HEIGHT + 5) * TILE_SIZE}));
        line.setPosition({(float)x * TILE_SIZE, 0});
        line.setFillColor(gridColor);
        window.draw(line);
    }

    for (int y = 0; y <= MAP_HEIGHT + 5; y++)
    {
        sf::RectangleShape line(sf::Vector2f({MAP_WIDTH * TILE_SIZE, thickness}));
        line.setPosition({0, (float)y * TILE_SIZE});
        line.setFillColor(gridColor);
        window.draw(line);
    }*/
}

void State::doUI()
{
    drawLives();

    drawChar(3, 0, CHAR_1);
    drawChar(4, 0, CHAR_U);
    drawChar(5, 0, CHAR_P);

    drawChar(9, 0, CHAR_H);
    drawChar(10, 0, CHAR_I);
    drawChar(11, 0, CHAR_G);
    drawChar(12, 0, CHAR_H);

    drawChar(14, 0, CHAR_S);
    drawChar(15, 0, CHAR_C);
    drawChar(16, 0, CHAR_O);
    drawChar(17, 0, CHAR_R);
    drawChar(18, 0, CHAR_E);

    drawScore(6, 1, score);

    if (score > highscore)
    {
        highscore = score;
    }

    drawScore(16, 1, highscore);
    drawRecentFruits();
}

void State::drawChar(int x, int y, sf::Vector2i charPos)
{
    sf::Sprite textSprite = createSprite(
        textures[1].texture,
        charPos,
        {(float)TILE_SIZE / TEXT_SIZE, (float)TILE_SIZE / TEXT_SIZE},
        1.f,
        TEXT_SIZE,
        false);

    textSprite.setPosition({(float)x * TILE_SIZE, (float)y * TILE_SIZE});
    window.draw(textSprite);
}

void State::drawScore(int x, int y, int score)
{
    std::string scoreString = std::to_string(score);
    int scoreLength = scoreString.length();
    int startX = x - scoreLength;

    for (size_t i = 0; i < scoreString.length(); i++)
    {
        char c = scoreString[i];
        drawChar(startX + i, y, CHAR_MAP.at(c));
    }
}

void State::drawLives()
{
    sf::Vector2i pacmanPos = pacman.PACMAN_TEX_MAP.at(PacMan::LEFT);
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

void State::drawFruit(float x, float y, sf::Vector2i fruitPos, float scaleFactor)
{
    sf::Sprite fruitSprite = createSprite(textures[0].texture, fruitPos, textures[0].scale, scaleFactor, TILE_SIZE / 2, true);
    fruitSprite.setPosition({x * TILE_SIZE, y * TILE_SIZE});
    window.draw(fruitSprite);
}

void State::drawRecentFruits()
{
    int recentFruitsSize = recentFruits.size();
    int startX = 24 - (recentFruitsSize - 1) * 2;

    for (size_t i = 0; i < recentFruits.size(); i++)
    {
        sf::Vector2i fruitPos = recentFruits[i];
        drawFruit((startX + (i * 2)) + 1.f, (MAP_HEIGHT + 4), fruitPos, 2.f);
    }
}

////////////
// Events //
////////////

void handle(const sf::Event::Closed &, State &gs)
{
    gs.window.close();
}

template <typename T>
void handle(const T &, State &gs)
{
    // eventi non gestiti
}

void handle(const sf::Event::KeyPressed &key, State &gs)
{
    PacMan::Direction newDirection = gs.pacman.direction;

    switch (key.scancode)
    {
    case sf::Keyboard::Scancode::Up:
        newDirection = PacMan::UP;
        break;
    case sf::Keyboard::Scancode::Down:
        newDirection = PacMan::DOWN;
        break;
    case sf::Keyboard::Scancode::Left:
        newDirection = PacMan::LEFT;
        break;
    case sf::Keyboard::Scancode::Right:
        newDirection = PacMan::RIGHT;
        break;
    default:
        return;
    }

    gs.pacman.setRotation(newDirection);
}

int main()
{

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned int w = desktop.size.x;
    unsigned int h = desktop.size.y;

    State gs(w, h, "Pac-Man");
    sf::Clock clock;

    if (!gs.getMap("../resources/default_map.txt"))
    {
        return 1;
    }

    while (gs.window.isOpen())
    {
        gs.window.handleEvents([&](const auto &event)
                               { handle(event, gs); });

        gs.update(clock.restart().asSeconds());
        gs.window.clear();
        gs.doUI();
        gs.doGraphics();
        gs.window.display();
    }

    return 0;
}