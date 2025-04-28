#include <SFML/Graphics.hpp>
#include "../includes/textures.hpp"
#include "../includes/global_values.hpp"
#include "../includes/PacMan.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <map>

struct TileData
{
    sf::Texture texture;
    sf::Vector2f scale;
};

struct State
{
    sf::RenderWindow window;
    std::map<int, TileData> mapTextures;
    PacMan pacman;
    char map[MAP_WIDTH][MAP_HEIGHT];

    State(unsigned w, unsigned h, std::string title);
    bool getMap(std::string mapPath);
    void update(float elapsed);
    void bounds();
    void doGraphics();
};

State::State(unsigned w, unsigned h, std::string title)
    : pacman()
{
    window = sf::RenderWindow(sf::VideoMode({w, h}), title);
    window.setPosition({0, 0});
    window.setFramerateLimit(60);

    sf::Texture temp;

    if (!temp.loadFromFile(STRAIGHT_LINE_H))
    {
        std::cerr << "Errore nel caricamento della texture" << std::endl;
        exit(1);
    }
    mapTextures[0].texture = temp;
    sf::Vector2u texSize = mapTextures[0].texture.getSize();
    mapTextures[0].scale = {(float)TILE_SIZE / texSize.x, (float)TILE_SIZE / texSize.y};

    if (!temp.loadFromFile(STRAIGHT_LINE_V))
    {
        std::cerr << "Errore nel caricamento della texture" << std::endl;
        exit(1);
    }
    mapTextures[1].texture = temp;
    mapTextures[1].scale = {(float)TILE_SIZE / texSize.x, (float)TILE_SIZE / texSize.y};

    if (!temp.loadFromFile(ANGLE_0))
    {
        std::cerr << "Errore nel caricamento della texture" << std::endl;
        exit(1);
    }
    mapTextures[2].texture = temp;
    mapTextures[2].scale = {(float)TILE_SIZE / texSize.x, (float)TILE_SIZE / texSize.y};

    if (!temp.loadFromFile(ANGLE_90))
    {
        std::cerr << "Errore nel caricamento della texture" << std::endl;
        exit(1);
    }
    mapTextures[3].texture = temp;
    mapTextures[3].scale = {(float)TILE_SIZE / texSize.x, (float)TILE_SIZE / texSize.y};

    if (!temp.loadFromFile(ANGLE_180))
    {
        std::cerr << "Errore nel caricamento della texture" << std::endl;
        exit(1);
    }
    mapTextures[4].texture = temp;
    mapTextures[4].scale = {(float)TILE_SIZE / texSize.x, (float)TILE_SIZE / texSize.y};

    if (!temp.loadFromFile(ANGLE_270))
    {
        std::cerr << "Errore nel caricamento della texture" << std::endl;
        exit(1);
    }
    mapTextures[5].texture = temp;
    mapTextures[5].scale = {(float)TILE_SIZE / texSize.x, (float)TILE_SIZE / texSize.y};
}

void State::update(float elapsed)
{
    pacman.move(elapsed);
    bounds();
}

void State::bounds()
{
    if (pacman.position.x < 0)
    {
        pacman.position.x = MAP_HEIGHT - 1;
    }
    else if (pacman.position.x > MAP_HEIGHT - 1)
    {
        pacman.position.x = 0;
    }

    if (pacman.position.y < 0)
    {
        pacman.position.y = MAP_WIDTH - 1;
    }
    else if (pacman.position.y > MAP_WIDTH - 1)
    {
        pacman.position.y = 0;
    }
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
    int r = 0;

    while (std::getline(mapFile, mapString))
    {
        for (int i = 0; i < mapString.size(); i++)
        {
            if (mapString[i] == 'P')
            {
                pacman.setPosition(r, i);
                map[r][i] = EMPTY_BLOCK;
            }
            else
            {
                map[r][i] = mapString[i];
            }
        }
        r++;
    }
    mapFile.close();
    return true;
}

void State::doGraphics()
{
    window.clear();
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
                sf::Sprite wall(mapTextures[1].texture);
                wall.setScale(mapTextures[1].scale);
                wall.setPosition({x, y});
                window.draw(wall);
                break;
            }
            case CORNER_0:
            {
                sf::Sprite wall(mapTextures[2].texture);
                wall.setScale(mapTextures[2].scale);
                wall.setPosition({x, y});
                window.draw(wall);
                break;
            }
            case CORNER_90:
            {
                sf::Sprite wall(mapTextures[3].texture);
                wall.setScale(mapTextures[3].scale);
                wall.setPosition({x, y});
                window.draw(wall);
                break;
            }
            case CORNER_180:
            {
                sf::Sprite wall(mapTextures[4].texture);
                wall.setScale(mapTextures[4].scale);
                wall.setPosition({x, y});
                window.draw(wall);
                break;
            }
            case CORNER_270:
            {
                sf::Sprite wall(mapTextures[5].texture);
                wall.setScale(mapTextures[5].scale);
                wall.setPosition({x, y});
                window.draw(wall);
                break;
            }
            case POWERPELLET:
            {
                sf::CircleShape powerpellet(TILE_SIZE / 2);
                powerpellet.setPosition({x + (TILE_SIZE / 2) - 16, y + (TILE_SIZE / 2) - 16});
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
    }

    pacman.draw(window);
    window.display();
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

void handle(const sf::Event::KeyPressed &key, State &state)
{
    switch (key.scancode)
    {
    case sf::Keyboard::Scancode::Up:
        state.pacman.setRotation(PacMan::UP);
        break;
    case sf::Keyboard::Scancode::Down:
        state.pacman.setRotation(PacMan::DOWN);
        break;
    case sf::Keyboard::Scancode::Left:
        state.pacman.setRotation(PacMan::LEFT);
        break;
    case sf::Keyboard::Scancode::Right:
        state.pacman.setRotation(PacMan::RIGHT);
        break;
    default:
        return;
    }
}

int main()
{
    State gs(MAP_WIDTH * TILE_SIZE, (MAP_HEIGHT + 5) * TILE_SIZE, "Pac-Man");
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
        gs.doGraphics();
    }

    return 0;
}