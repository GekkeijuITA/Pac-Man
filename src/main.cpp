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
    sf::Vector2i cursorPos;
    int LAST_OPTION;

    switch (key.scancode)
    {
    case sf::Keyboard::Scancode::Up:

        if (gs.pause)
        {
            cursorPos = gs.pauseMenu.getCursorPosition();
            if (cursorPos.y > FIRST_OPTION)
            {
                gs.pauseMenu.setCursorPosition(cursorPos.x, cursorPos.y - 2);
            }
        }
        else if (gs.victory)
        {
            cursorPos = gs.victoryMenu.getCursorPosition();
            if (cursorPos.y > FIRST_OPTION)
            {
                gs.victoryMenu.setCursorPosition(cursorPos.x, cursorPos.y - 2);
            }
        }
        else
            newDirection = UP;
        break;
    case sf::Keyboard::Scancode::Down:
        if (gs.pause)
        {
            cursorPos = gs.pauseMenu.getCursorPosition();
            LAST_OPTION = FIRST_OPTION + gs.pauseMenu.getOptionsSize();
            if (cursorPos.y < LAST_OPTION)
            {
                gs.pauseMenu.setCursorPosition(cursorPos.x, cursorPos.y + 2);
            }
        }
        else if (gs.victory)
        {
            cursorPos = gs.victoryMenu.getCursorPosition();
            LAST_OPTION = FIRST_OPTION + gs.victoryMenu.getOptionsSize();
            if (cursorPos.y < LAST_OPTION)
            {
                gs.victoryMenu.setCursorPosition(cursorPos.x, cursorPos.y + 2);
            }
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
        if (gs.pause && !gs.victory && !gs.gameOver)
        {
            LAST_OPTION = FIRST_OPTION + gs.pauseMenu.getOptionsSize() + 1;
            cursorPos = gs.pauseMenu.getCursorPosition();
            if (cursorPos.y == FIRST_OPTION)
            { // Continue
                gs.pause = false;
            }
            else if (cursorPos.y == FIRST_OPTION + 2)
            { // Restart
                gs.resetRound();
                gs.pause = false;
            }
            else if (cursorPos.y == LAST_OPTION)
            { // Quit
                gs.window.close();
            }
        }
        else if (gs.gameOver)
        {
            std::cout << "Going to menu..." << std::endl;
        }
        else if (gs.victory)
        {
            LAST_OPTION = FIRST_OPTION + gs.victoryMenu.getOptionsSize();
            cursorPos = gs.victoryMenu.getCursorPosition();

            if (cursorPos.y == FIRST_OPTION)
            { // Restart
                gs.resetGame();
            }
            else if (cursorPos.y == LAST_OPTION)
            { // Quit
                gs.window.close();
            }
        }
        break;
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