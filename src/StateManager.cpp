#include "../includes/StateManager.hpp"

void handle(const sf::Event::KeyPressed &key, StateManager &sm, GameState &gs)
{
    switch (sm.currentMode)
    {
    case StateManager::NORMAL_GAME:
    {
        Direction newDirection = gs.pacman.direction;

        switch (key.scancode)
        {
        case sf::Keyboard::Scancode::Up:

            if (gs.pause)
            {
                if (gs.pauseMenu.cursorIndex > 0)
                {
                    gs.pauseMenu.cursorIndex--;
                }
            }
            else if (gs.victory)
            {
                if (gs.victoryMenu.cursorIndex > 0)
                {
                    gs.victoryMenu.cursorIndex--;
                }
            }
            else
                newDirection = UP;
            break;
        case sf::Keyboard::Scancode::Down:
            if (gs.pause)
            {
                if (gs.pauseMenu.cursorIndex < gs.pauseMenu.getOptionsSize() - 1)
                {
                    gs.pauseMenu.cursorIndex++;
                }
            }
            else if (gs.victory)
            {
                if (gs.victoryMenu.cursorIndex < gs.victoryMenu.getOptionsSize() - 1)
                {
                    gs.victoryMenu.cursorIndex++;
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
                gs.pauseMenu.cursorIndex = 0;
                gs.pause = !gs.pause;
            }
            break;
        case sf::Keyboard::Scancode::Enter:
            if (gs.pause && !gs.victory && !gs.gameOver)
            {
                switch (gs.pauseMenu.cursorIndex)
                {
                case 0:
                    gs.pause = false;
                    break;
                case 1:
                    gs.resetRound();
                    break;
                case 2:
                    gs.saveHighscore();
                    sm.currentMode = StateManager::MAIN_MENU;
                    sm.mainMenuState->getHighscore();
                    break;
                default:
                    break;
                }
            }
            else if (gs.gameOver)
            {
                sm.currentMode = StateManager::MAIN_MENU;
                sm.mainMenuState->getHighscore();
            }
            else if (gs.victory)
            {
                switch (gs.victoryMenu.cursorIndex)
                {
                case 0: // Restart game
                    gs.resetGame();
                    break;
                case 1: // Return to menu
                    sm.currentMode = StateManager::MAIN_MENU;
                    sm.mainMenuState->getHighscore();
                    break;
                default:
                    break;
                }
            }
            break;
        default:
            return;
        }

        gs.pacman.setRotation(newDirection);
        break;
    }
    case StateManager::MAIN_MENU:
        if (key.scancode == sf::Keyboard::Scancode::Up)
        {
            if (sm.mainMenuState->cursorIndex > 0)
            {
                sm.mainMenuState->cursorIndex--;
            }
        }
        else if (key.scancode == sf::Keyboard::Scancode::Down)
        {
            if (sm.mainMenuState->cursorIndex < sm.mainMenuState->options.size() - 1)
            {
                sm.mainMenuState->cursorIndex++;
            }
        }
        else if (key.scancode == sf::Keyboard::Scancode::Enter)
        {
            switch (sm.mainMenuState->cursorIndex)
            {
            case 0:
                sm.currentMode = StateManager::LEVEL_SELECTOR;
                break;
            case 1:
                // sm.currentMode = StateManager::MAP_EDITOR;
                std::cout << "MAP EDITOR" << std::endl;
                break;
            case 2:
                sm.window.close();
                break;
            default:
                break;
            }
        }
        break;
    case StateManager::LEVEL_SELECTOR:
        if (key.scancode == sf::Keyboard::Scancode::Up)
        {
            if (sm.levelSelectorState->cursorPosition.y > 0)
            {
                sm.levelSelectorState->cursorPosition.y--;
            }
        }
        else if (key.scancode == sf::Keyboard::Scancode::Down)
        {
            int rows = sm.levelSelectorState->maxRows = std::min(
                sm.levelSelectorState->maxRows,
                static_cast<int>(sm.levelSelectorState->maps.size()));

            if (sm.levelSelectorState->cursorPosition.y < rows - 1)
            {
                sm.levelSelectorState->cursorPosition.y++;
            }
        }
        else if (key.scancode == sf::Keyboard::Scancode::Left)
        {
            if (sm.levelSelectorState->cursorPosition.x > 0)
            {
                sm.levelSelectorState->cursorPosition.x--;
            }
        }
        else if (key.scancode == sf::Keyboard::Scancode::Right)
        {
            if (sm.levelSelectorState->cursorPosition.x < sm.levelSelectorState->maps[sm.levelSelectorState->cursorPosition.y].size() - 1)
            {
                sm.levelSelectorState->cursorPosition.x++;
            }
        }
        else if (key.scancode == sf::Keyboard::Scancode::Escape)
        {
            sm.currentMode = StateManager::MAIN_MENU;
            sm.mainMenuState->getHighscore();
        }
        else if (key.scancode == sf::Keyboard::Scancode::Enter)
        {
            std::string mapPath = sm.levelSelectorState->maps[sm.levelSelectorState->cursorPosition.y][sm.levelSelectorState->cursorPosition.x].path;
            sm.initGame(mapPath);
        }
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

    window = sf::RenderWindow(sf::VideoMode({windowWidth, windowHeight}), title, sf::Style::Close | sf::Style::Titlebar);
    window.setPosition({static_cast<int>((w - windowWidth) / 2),
                        static_cast<int>((h - windowHeight) / 2)});
    window.setFramerateLimit(60);

    sf::View view;
    view.setSize({MAP_WIDTH * TILE_SIZE, (MAP_HEIGHT + 5) * TILE_SIZE});
    view.setCenter({view.getSize().x / 2.f, view.getSize().y / 2.f});
    window.setView(view);

    mainMenuState = std::make_unique<MainMenuState>(window, *this);
    levelSelectorState = std::make_unique<LevelSelectorState>(window);
};

void StateManager::update(float elapsed)
{
    switch (currentMode)
    {
    case NORMAL_GAME:
        gameState->update(elapsed);
        break;
    default:
        break;
    }
}

void StateManager::doGraphics()
{
    switch (currentMode)
    {
    case MAIN_MENU:
        mainMenuState->draw();
        break;
    case NORMAL_GAME:
        gameState->doGraphics();
        gameState->doUI();
        break;
    case LEVEL_SELECTOR:
        levelSelectorState->draw();
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