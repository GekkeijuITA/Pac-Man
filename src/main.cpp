#include <SFML/Graphics.hpp>

#include <fstream>
#include <iostream>
#include <string>

#define MAP_WIDTH 28
#define MAP_HEIGHT 31
#define TILE_SIZE 32

struct State
{
    sf::RenderWindow window;

    State(unsigned w, unsigned h, std::string title)
    {
        window = sf::RenderWindow(sf::VideoMode({w, h}), title);
    }
};

void handle(const sf::Event::Closed &, State &gs)
{
    gs.window.close();
}

template <typename T>
void handle(const T &, State &gs)
{
    // eventi non gestiti
}

void doGraphics(State &gs, int (&map)[MAP_HEIGHT][MAP_WIDTH])
{
    gs.window.clear();

    for (int r = 0; r < MAP_HEIGHT; r++)
    {
        float y = (r + 3) * TILE_SIZE; // Lasciamo le prime due righe per il punteggio
        for (int c = 0; c < MAP_WIDTH; c++)
        {
            float x = c * TILE_SIZE;
            switch (map[r][c])
            {
            // 0: pacdot, 1: muro, 2:powerpellet, 3:portecasafantasmi, 4: blocconero
            case 0:
            {
                sf::CircleShape pacdot(TILE_SIZE / 10.f);
                pacdot.setPosition({x + (TILE_SIZE / 2) - 4, y + (TILE_SIZE / 2) - 4});
                pacdot.setFillColor(sf::Color(255,185,176));
                gs.window.draw(pacdot);
                break;
            }
            case 1:
            {
                sf::RectangleShape wall({TILE_SIZE, TILE_SIZE});
                wall.setPosition({x, y});
                wall.setFillColor(sf::Color::Blue);
                gs.window.draw(wall);
                break;
            }
            case 2:
            {
                sf::CircleShape powerpellet(TILE_SIZE / 2);
                powerpellet.setPosition({x + (TILE_SIZE / 2) - 16, y + (TILE_SIZE / 2) - 16});
                powerpellet.setFillColor(sf::Color(255,185,176));
                gs.window.draw(powerpellet);
                break;
            }
            case 3:
            {
                sf::RectangleShape ghostDoor({TILE_SIZE, TILE_SIZE / 4});
                ghostDoor.setPosition({x, y + TILE_SIZE / 1.8f});
                ghostDoor.setFillColor(sf::Color(255,203,255));
                gs.window.draw(ghostDoor);
                break;
            }
            case 4:
            {
                sf::RectangleShape emptyBlock({TILE_SIZE, TILE_SIZE});
                emptyBlock.setPosition({x, y});
                emptyBlock.setFillColor(sf::Color::Black);
                gs.window.draw(emptyBlock);
                break;
            }
            default:
                break;
            }
        }
    }

    gs.window.display();
}

bool getMap(std::string mapPath, int (&map)[MAP_HEIGHT][MAP_WIDTH])
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
            map[r][i] = mapString[i] - '0';
        }
        r++;
    }
    mapFile.close();
    return true;
}

int main()
{
    State gs(MAP_WIDTH * TILE_SIZE, (MAP_HEIGHT + 5) * TILE_SIZE, "Pac-Man");

    int map[MAP_HEIGHT][MAP_WIDTH];
    if (!getMap("../assets/default_map.txt", map))
    {
        return 1;
    }

    while (gs.window.isOpen())
    {
        gs.window.handleEvents([&](const auto &event)
                               { handle(event, gs); });

        doGraphics(gs, map);
    }

    return 0;
}