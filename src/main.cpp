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
    srand(time(0));
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