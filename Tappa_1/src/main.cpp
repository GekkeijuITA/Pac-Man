#include <SFML/Graphics.hpp>
#include "../includes/global_values.hpp"

#include <iostream>
#include <string>
#include <map>

struct State
{
    sf::RenderWindow window;
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
        windowWidth = w * scaleFactor;
        windowHeight = windowWidth / mapRatio;
    }
    else
    {
        windowHeight = h * scaleFactor;
        windowWidth = windowHeight * mapRatio;
    }

    window = sf::RenderWindow(sf::VideoMode({windowWidth, windowHeight}), title);
    window.setPosition({
        static_cast<int>((w - windowWidth) / 2), 
        static_cast<int>((h - windowHeight) / 2)
    });
    window.setFramerateLimit(60);
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

    while (gs.window.isOpen())
    {
        gs.window.handleEvents([&](const auto &event)
                               { handle(event, gs); });
    }

    return 0;
}