#include "../includes/StateManager.hpp"

void handle(const sf::Event::KeyPressed &key, StateManager &sm, GameState &gs)
{
    switch (sm.currentMode)
    {
    case StateManager::NORMAL_GAME: 
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
                    sm.window.close();
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
                    sm.window.close();
                }
            }
            break;
        default:
            return;
        }

        gs.pacman.setRotation(newDirection);
        break;
    }
}

void handle(const sf::Event::Closed &close, StateManager &sm, GameState &gs)
{
    sm.window.close();
}

template <typename T>
void handle(const T &, StateManager &sm, GameState &gs)
{
    // eventi non gestiti
}

StateManager::StateManager(unsigned w, unsigned h, std::string title) : w(w), h(h), title(title)
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
    window.setPosition({static_cast<int>((w - windowWidth) / 2),
                        static_cast<int>((h - windowHeight) / 2)});
    window.setFramerateLimit(60);

    sf::View view;
    view.setSize({MAP_WIDTH * TILE_SIZE, (MAP_HEIGHT + 5) * TILE_SIZE});
    view.setCenter({view.getSize().x / 2.f, view.getSize().y / 2.f});
    window.setView(view);
};

void StateManager::update(float elapsed)
{
    switch (currentMode)
    {
    case MAIN_MENU:
        // Update main menu state
        break;
    case NORMAL_GAME:
        gameState->update(elapsed);
        break;
    }
}

void StateManager::doGraphics()
{
    switch (currentMode)
    {
    case MAIN_MENU:
        // Draw main menu state
        break;
    case NORMAL_GAME:
        gameState->doGraphics();
        gameState->doUI();
        break;
    }
}

void StateManager::handleInputs()
{
    window.handleEvents([&](const auto &event)
                        { handle(event, *this, *gameState); });
}

void StateManager::initGame(std::string mapPath)
{
    currentMode = NORMAL_GAME;
    gameState = std::make_unique<GameState>(window, mapPath, *this);
}