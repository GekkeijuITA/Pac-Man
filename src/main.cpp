#include <SFML/Graphics.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

#define MAP_WIDTH 28
#define MAP_HEIGHT 31
#define TILE_SIZE 32

int main()
{
    std::fstream mapFile;
    mapFile.open("../assets/default_map.txt", std::ios::in);
    if (!mapFile.is_open())
    {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::string mapString;
    int map[MAP_HEIGHT][MAP_WIDTH];
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

    sf::RenderWindow window(sf::VideoMode({MAP_WIDTH * TILE_SIZE, (MAP_HEIGHT + 5) * TILE_SIZE}), "Pac-Man");

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();

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
                    window.draw(pacdot);
                    break;
                }
                case 1:
                {
                    sf::RectangleShape wall({TILE_SIZE, TILE_SIZE});
                    wall.setPosition({x, y});
                    wall.setFillColor(sf::Color::Blue);
                    window.draw(wall);
                    break;
                }
                case 2:
                {
                    sf::CircleShape powerpellet(TILE_SIZE/2);
                    powerpellet.setPosition({x + (TILE_SIZE / 2)-16, y + (TILE_SIZE / 2)-16});
                    window.draw(powerpellet);
                    break;
                }
                case 3:
                {
                    sf::RectangleShape ghostDoor({TILE_SIZE,TILE_SIZE/4});
                    ghostDoor.setPosition({x, y + TILE_SIZE/1.8f});
                    ghostDoor.setFillColor(sf::Color::Red);
                    window.draw(ghostDoor);
                    break;
                }
                case 4:
                {
                    sf::RectangleShape emptyBlock({TILE_SIZE,TILE_SIZE});
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

        window.display();
    }

    return 0;
}