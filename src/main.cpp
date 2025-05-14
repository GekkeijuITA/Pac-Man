#include "../includes/GameState.hpp"

////////////
// Events //
////////////

void handle(const sf::Event::Closed &, GameState &gs)
{
    gs.window.close();
}

template <typename T>
void handle(const T &, GameState &gs)
{
    // eventi non gestiti
}

void handle(const sf::Event::KeyPressed &key, GameState &gs)
{
    Direction newDirection = gs.pacman.direction;

    switch (key.scancode)
    {
    case sf::Keyboard::Scancode::Up:
        if (gs.pause && gs.pauseMenu.getCursorPosition().y > FIRST_OPTION)
        {
            gs.pauseMenu.setCursorPosition(gs.pauseMenu.getCursorPosition().x, gs.pauseMenu.getCursorPosition().y - 2);
        }
        else
            newDirection = UP;
        break;
    case sf::Keyboard::Scancode::Down:
        if (gs.pause && gs.pauseMenu.getCursorPosition().y < LAST_OPTION)
        {
            gs.pauseMenu.setCursorPosition(gs.pauseMenu.getCursorPosition().x, gs.pauseMenu.getCursorPosition().y + 2);
        }
        else
            newDirection = DOWN;
        break;
    case sf::Keyboard::Scancode::Left:
        if (!gs.pause)
            newDirection = LEFT;
        break;
    case sf::Keyboard::Scancode::Right:
        if (!gs.pause)
            newDirection = RIGHT;
        break;
    case sf::Keyboard::Scancode::Escape:
        if (!gs.gameOver)
        {
            gs.pauseMenu.setCursorPosition(10, FIRST_OPTION);
            gs.pause = !gs.pause;
        }
        break;
    case sf::Keyboard::Scancode::Enter:
        if (gs.pause)
        {
            switch (gs.pauseMenu.getCursorPosition().y)
            {
            case FIRST_OPTION:
                gs.pause = false;
                break;
            case FIRST_OPTION + 2:
                gs.resetRound();
                gs.pause = false;
                break;
            case LAST_OPTION:
                gs.window.close();
                break;
            default:
                break;
            }
        }
        else if (gs.gameOver)
        {
            std::cout << "Going to menu..." << std::endl;
        }
    default:
        return;
    }

    gs.pacman.setRotation(newDirection);
}

/*void handle(const sf::Event::FocusGained &, State &gs)
{
    gs.pause = false;
}

void handle(const sf::Event::FocusLost &, State &gs)
{
    gs.pause = true;
}*/

int main()
{
    srand(time(0));
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned int w = desktop.size.x;
    unsigned int h = desktop.size.y;

    GameState gs(w, h, "Pac-Man", "../resources/default_map.txt");
    sf::Clock clock;

    while (gs.window.isOpen())
    {
        gs.window.handleEvents([&](const auto &event)
                               { handle(event, gs); });

        float elapsed = clock.restart().asSeconds();

        gs.update(elapsed);
        gs.window.clear();
        gs.doGraphics();
        gs.doUI();
        gs.window.display();
    }

    return 0;
}