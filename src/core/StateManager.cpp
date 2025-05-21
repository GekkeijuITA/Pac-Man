#include "../../includes/core/StateManager.hpp"

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
        {

            if (gs.pause)
            {
                gs.pauseMenu.moveCursorUp();
            }
            else if (gs.victory)
            {
                gs.victoryMenu.moveCursorUp();
            }
            else
            {
                newDirection = UP;
            }
            break;
        }
        case sf::Keyboard::Scancode::Down:
        {
            if (gs.pause)
            {
                gs.pauseMenu.moveCursorDown();
            }
            else if (gs.victory)
            {
                gs.victoryMenu.moveCursorDown();
            }
            else
                newDirection = DOWN;
            break;
        }
        case sf::Keyboard::Scancode::Left:
        {
            if (!gs.pause)
                newDirection = LEFT;
            break;
        }
        case sf::Keyboard::Scancode::Right:
        {
            if (!gs.pause)
                newDirection = RIGHT;
            break;
        }
        case sf::Keyboard::Scancode::Escape:
        {
            if (!gs.gameOver)
            {
                gs.pauseMenu.cursorIndex = 0;
                gs.pause = !gs.pause;
            }
            break;
        }
        case sf::Keyboard::Scancode::Enter:
        {
            if (gs.pause && !gs.victory && !gs.gameOver)
            {
                gs.pauseMenu.executeOption();
            }
            else if (gs.gameOver)
            {
                sm.currentMode = StateManager::MAIN_MENU;
                sm.mainMenuState->getHighscore();
            }
            else if (gs.victory)
            {
                gs.victoryMenu.executeOption();
            }
            break;
        }
        default:
            return;
        }

        gs.pacman.setRotation(newDirection);
        break;
    }
    case StateManager::MAIN_MENU:
    {
        if (key.scancode == sf::Keyboard::Scancode::Up)
        {
            sm.mainMenuState->menu.moveCursorUp();
        }
        else if (key.scancode == sf::Keyboard::Scancode::Down)
        {
            sm.mainMenuState->menu.moveCursorDown();
        }
        else if (key.scancode == sf::Keyboard::Scancode::Enter)
        {
            sm.mainMenuState->menu.executeOption();
        }
        break;
    }
    case StateManager::LEVEL_SELECTOR:
    {
        if (key.scancode == sf::Keyboard::Scancode::Up)
        {
            sm.levelSelectorState->moveCursorUp();
        }
        else if (key.scancode == sf::Keyboard::Scancode::Down)
        {
            sm.levelSelectorState->moveCursorDown();
        }
        else if (key.scancode == sf::Keyboard::Scancode::Left)
        {
            sm.levelSelectorState->moveCursorLeft();
        }
        else if (key.scancode == sf::Keyboard::Scancode::Right)
        {
            sm.levelSelectorState->moveCursorRight();
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
    case StateManager::MAP_EDITOR:
    {
        if (key.scancode == sf::Keyboard::Scancode::Up)
        {
            sm.mapEditor->menu.moveCursorUp();
        }
        else if (key.scancode == sf::Keyboard::Scancode::Down)
        {
            sm.mapEditor->menu.moveCursorDown();
        }
        else if (key.scancode == sf::Keyboard::Scancode::Enter)
        {
            sm.mapEditor->menu.executeOption();
        }
    }
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
    mapEditor = std::make_unique<MapEditor>(window, *this);
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
    case MAP_EDITOR:
        mapEditor->doGraphics();
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