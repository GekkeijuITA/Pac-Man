#include <SFML/Graphics.hpp>
#include "../includes/textures.hpp"
#include "../includes/global_values.hpp"
#include "../includes/PacMan.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <map>

struct TileData {
    sf::Texture texture;
    sf::Vector2f scale;
};

struct State
{
    sf::RenderWindow window;
    std::map<int, TileData> mapTextures;


    State(unsigned w, unsigned h, std::string title)
    {
        window = sf::RenderWindow(sf::VideoMode({w, h}), title);
        sf::Texture temp;

        if(!temp.loadFromFile(STRAIGHT_LINE_H))
        {
            std::cerr << "Errore nel caricamento della texture" << std::endl;
            exit(1);
        }
        mapTextures[0].texture = temp;
        sf::Vector2u texSize = mapTextures[0].texture.getSize();
        mapTextures[0].scale = {(float) TILE_SIZE / texSize.x, (float) TILE_SIZE / texSize.y};

        if(!temp.loadFromFile(STRAIGHT_LINE_V))
        {
            std::cerr << "Errore nel caricamento della texture" << std::endl;
            exit(1);
        }
        mapTextures[1].texture = temp;
        mapTextures[1].scale = {(float) TILE_SIZE / texSize.x, (float) TILE_SIZE / texSize.y};

        if(!temp.loadFromFile(ANGLE_0))
        {
            std::cerr << "Errore nel caricamento della texture" << std::endl;
            exit(1);
        }
        mapTextures[2].texture = temp;
        mapTextures[2].scale = {(float) TILE_SIZE / texSize.x, (float) TILE_SIZE / texSize.y};

        if(!temp.loadFromFile(ANGLE_90))
        {
            std::cerr << "Errore nel caricamento della texture" << std::endl;
            exit(1);
        }
        mapTextures[3].texture = temp;
        mapTextures[3].scale = {(float) TILE_SIZE / texSize.x, (float) TILE_SIZE / texSize.y};

        if(!temp.loadFromFile(ANGLE_180))
        {
            std::cerr << "Errore nel caricamento della texture" << std::endl;
            exit(1);
        }
        mapTextures[4].texture = temp;
        mapTextures[4].scale = {(float) TILE_SIZE / texSize.x, (float) TILE_SIZE / texSize.y};

        if(!temp.loadFromFile(ANGLE_270))
        {
            std::cerr << "Errore nel caricamento della texture" << std::endl;
            exit(1);
        }
        mapTextures[5].texture = temp;
        mapTextures[5].scale = {(float) TILE_SIZE / texSize.x, (float) TILE_SIZE / texSize.y};
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

void doGraphics(State &gs, char (&map)[MAP_HEIGHT][MAP_WIDTH], PacMan &pacman)
{
    gs.window.clear();
    for (int r = 0; r < MAP_HEIGHT; r++)
    {
        float y = (r + 3) * TILE_SIZE; // Lasciamo le prime due righe per il punteggi
        for (int c = 0; c < MAP_WIDTH; c++)
        {
            float x = c * TILE_SIZE;
            /*
            0=pacdot;
            1=muroorizz;
            2=murovert;
            3=angolo0;
            4=angolo90;
            5=angolo180;
            6=angolo270;
            7=powerpellet;
            8=portafantasmi;
            9=empty;
            */
            switch (map[r][c])
            {
            case '0':
            {
                sf::CircleShape pacdot(TILE_SIZE / 10.f);
                pacdot.setPosition({x + (TILE_SIZE / 2) - 4, y + (TILE_SIZE / 2) - 4});
                pacdot.setFillColor(sf::Color(255, 185, 176));
                gs.window.draw(pacdot);
                break;
            }
            case '1':
            {
                sf::Sprite wall(gs.mapTextures[0].texture);
                wall.setScale(gs.mapTextures[0].scale);
                wall.setPosition({x, y});
                gs.window.draw(wall);
                break;
            }
            case '2':
            {
                sf::Sprite wall(gs.mapTextures[1].texture);
                wall.setScale(gs.mapTextures[1].scale);
                wall.setPosition({x, y});
                gs.window.draw(wall);
                break;
            }
            case '3':
            {
                sf::Sprite wall(gs.mapTextures[2].texture);
                wall.setScale(gs.mapTextures[2].scale);
                wall.setPosition({x, y});
                gs.window.draw(wall);
                break;
            }
            case '4':
            {
                sf::Sprite wall(gs.mapTextures[3].texture);
                wall.setScale(gs.mapTextures[3].scale);
                wall.setPosition({x, y});
                gs.window.draw(wall);
                break;
            }
            case '5':
            {
                sf::Sprite wall(gs.mapTextures[4].texture);
                wall.setScale(gs.mapTextures[4].scale);
                wall.setPosition({x, y});
                gs.window.draw(wall);
                break;
            }
            case '6':
            {
                sf::Sprite wall(gs.mapTextures[5].texture);
                wall.setScale(gs.mapTextures[5].scale);
                wall.setPosition({x, y});
                gs.window.draw(wall);
                break;
            }
            case '7':
            {
                sf::CircleShape powerpellet(TILE_SIZE / 2);
                powerpellet.setPosition({x + (TILE_SIZE / 2) - 16, y + (TILE_SIZE / 2) - 16});
                powerpellet.setFillColor(sf::Color(255, 185, 176));
                gs.window.draw(powerpellet);
                break;
            }
            case '8':
            {
                sf::RectangleShape ghostDoor({TILE_SIZE, TILE_SIZE / 4});
                ghostDoor.setPosition({x, y + TILE_SIZE / 1.8f});
                ghostDoor.setFillColor(sf::Color(255, 203, 255));
                gs.window.draw(ghostDoor);
                break;
            }
            case '9':
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

    pacman.draw(gs.window);
    gs.window.display();
}

bool getMap(std::string mapPath, char (&map)[MAP_HEIGHT][MAP_WIDTH])
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
            map[r][i] = mapString[i];
        }
        r++;
    }
    mapFile.close();
    return true;
}

int main()
{
    State gs(MAP_WIDTH * TILE_SIZE, (MAP_HEIGHT + 5) * TILE_SIZE, "Pac-Man");

    char map[MAP_HEIGHT][MAP_WIDTH];
    if (!getMap("../resources/default_map.txt", map))
    {
        return 1;
    }

    PacMan pacman;

    while (gs.window.isOpen())
    {
        gs.window.handleEvents([&](const auto &event)
                               { handle(event, gs); });

        doGraphics(gs, map, pacman);
    }

    return 0;
}