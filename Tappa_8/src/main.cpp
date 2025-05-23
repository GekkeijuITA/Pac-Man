#include "../includes/State.hpp"

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
    Direction newDirection = gs.pacman.direction;

    switch (key.scancode)
    {
    case sf::Keyboard::Scancode::Up:
        newDirection = UP;
        break;
    case sf::Keyboard::Scancode::Down:
        newDirection = DOWN;
        break;
    case sf::Keyboard::Scancode::Left:
        newDirection = LEFT;
        break;
    case sf::Keyboard::Scancode::Right:
        newDirection = RIGHT;
        break;
    default:
        return;
    }

    gs.pacman.setRotation(newDirection);
}

void handle(const sf::Event::FocusGained &, State &gs)
{
    gs.pause = false;
}

void handle(const sf::Event::FocusLost &, State &gs)
{
    gs.pause = true;
}

int main()
{
    srand(time(0));
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned int w = desktop.size.x;
    unsigned int h = desktop.size.y;

    State gs(w, h, "Pac-Man");
    sf::Clock clock;

    if (!gs.getMap("../Tappa_8/resources/default_map.txt"))
    {
        return 1;
    }

    while (gs.window.isOpen())
    {
        gs.window.handleEvents([&](const auto &event)
                               { handle(event, gs); });

        float elapsed = clock.restart().asSeconds();
        
        gs.update(elapsed);
        gs.window.clear();
        gs.doUI();
        gs.doGraphics();
        gs.window.display();
    }

    return 0;
}