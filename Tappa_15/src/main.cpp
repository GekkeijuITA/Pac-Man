#include "../includes/core/StateManager.hpp"
#include "../includes/core/TileFactory.hpp"
#include "../includes/core/SoundManager.hpp"

int main()
{
    srand(time(0));
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned int w = desktop.size.x;
    unsigned int h = desktop.size.y;

    StateManager sm(w, h, "Pac-Man");
    TileFactory::getIstance();

    sf::Clock clock;

    while (sm.window.isOpen())
    {
        sm.handleInputs();
        float elapsed = clock.restart().asSeconds();

        sm.update(elapsed);
        sm.window.clear();
        sm.doGraphics();
        sm.window.display();
    }

    TileFactory::getIstance().cleanUp();

    return 0;
}