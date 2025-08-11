#include "../includes/core/StateManager.hpp"
#include "../includes/lib/TileFactory.hpp"

int main()
{
    srand(time(0));
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned int w = desktop.size.x;
    unsigned int h = desktop.size.y;

    //std::cout << "Current path: " << std::filesystem::current_path() << std::endl;

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