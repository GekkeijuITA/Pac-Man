#include <SFML/Graphics.hpp>
#include "lib/textures.hpp"
#include "lib/global_values.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <cmath>

struct TileData
{
    sf::Texture texture;
    sf::Vector2f scale;
};

struct State
{
    sf::RenderWindow window;
    std::map<int, TileData> mapTextures;
    char map[MAP_WIDTH][MAP_HEIGHT];

    State(unsigned w, unsigned h, std::string title);
};

State::State(unsigned w, unsigned h, std::string title)
{
    float mapRatio = (float)(MAP_WIDTH) / (MAP_HEIGHT + 5);
    float screenRatio = (float)w / h;

    float scaleFactor = 0.7f;
    unsigned int windowWidth, windowHeight;

    if (mapRatio > screenRatio)
    {
        // mappa più larga dello schermo
        windowWidth = w * scaleFactor;
        windowHeight = windowWidth / mapRatio;
    }
    else
    {
        // mappa più alta dello schermo
        windowHeight = h * scaleFactor;
        windowWidth = windowHeight * mapRatio;
    }

    window = sf::RenderWindow(sf::VideoMode({windowWidth, windowHeight}), title);
    window.setPosition({
        static_cast<int>((w - windowWidth) / 2), 
        static_cast<int>((h - windowHeight) / 2)
    });
    window.setFramerateLimit(60);

    sf::View view;
    view.setSize({MAP_WIDTH * TILE_SIZE, (MAP_HEIGHT + 5) * TILE_SIZE});
    view.setCenter({view.getSize().x / 2.f, view.getSize().y / 2.f});
    window.setView(view);
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

int main()
{

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned int w = desktop.size.x;
    unsigned int h = desktop.size.y;

    State gs(w, h, "Pac-Man");
    sf::Clock clock;

    while (gs.window.isOpen())
    {
        gs.window.handleEvents([&](const auto &event)
                               { handle(event, gs); });
    }

    return 0;
}