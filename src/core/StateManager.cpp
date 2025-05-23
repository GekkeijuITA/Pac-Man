#include "../../includes/core/StateManager.hpp"

void handle(const sf::Event::KeyPressed &key, StateManager &sm, GameState &gs)
{
    switch (sm.currentMode)
    {
    case StateManager::NORMAL_GAME:
    {
        if (gs.pause)
        {
            gs.pauseMenu.handle(key);
            return;
        }

        if (gs.gameOver)
        {
            gs.victoryMenu.handle(key);
            return;
        }

        Direction newDirection = gs.pacman.direction;

        switch (key.scancode)
        {
        case sf::Keyboard::Scancode::Up:
        {
            if (!gs.pause)
                newDirection = UP;
            break;
        }
        case sf::Keyboard::Scancode::Down:
        {
            if (!gs.pause)
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
                gs.pause = !gs.pause;
                gs.pauseMenu.cursorIndex = 0;
            }
            break;
        }
        case sf::Keyboard::Scancode::Enter:
        {
            if (gs.gameOver)
            {
                sm.currentMode = StateManager::MAIN_MENU;
                sm.mainMenuState->getHighscore();
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
        sm.mainMenuState->menu.handle(key);
        break;
    }
    case StateManager::LEVEL_SELECTOR:
    {
        sm.levelSelectorState->handle(key);
        break;
    }
    case StateManager::MAP_EDITOR:
    {
        if (sm.mapEditor->currentMode == MapEditor::CREATE)
        {
            if (sm.mapEditor->create->optionsMenu)
            {
                sm.mapEditor->create->menu.handle(key);
            }
            sm.mapEditor->create->handle(key);
        }
        else if (sm.mapEditor->currentMode == MapEditor::EDIT)
        {
            sm.mapEditor->levelSelectorState->handle(key);
        }
        else
        {
            sm.mapEditor->menu.handle(key);
        }
        break;
    }
    }
}

// https://www.sfml-dev.org/tutorials/3.0/graphics/view/#coordinates-conversions
void handle(const sf::Event::MouseMoved &mouseMoved, MapEditor &me)
{
    if (me.currentMode == me.CREATE && !me.create->optionsMenu)
    {
        me.create->handle(mouseMoved);
    }
}

void handle(const sf::Event::MouseButtonPressed &mouseButton, MapEditor &me)
{
    if (me.currentMode == me.CREATE)
    {
        me.create->handle(mouseButton);
    }
}

void handle(const sf::Event::Closed &close, StateManager &sm, GameState &gs)
{
    sm.window.close();
}

void handle(const sf::Event::TextEntered &textEntered, MapEditor &me)
{
    if (me.currentMode == me.CREATE && me.create->writingNameMap)
    {
        me.create->handle(textEntered);
    }
}

template <typename T>
void handle(const T &, StateManager &sm, GameState &gs)
{
    // eventi non gestiti
}

template <typename T>
void handle(const T &, MapEditor &me)
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

    mapEditor = std::make_unique<MapEditor>(window, *this);
    mainMenuState = std::make_unique<MainMenuState>(window, *this, *mapEditor);
    levelSelectorState = std::make_unique<LevelSelectorState>(window, *this, *mapEditor);
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
                        { handle(event, *this, *gameState); handle(event, *mapEditor); });
}

void StateManager::initGame(std::string mapPath)
{
    currentMode = NORMAL_GAME;
    gameState = std::make_unique<GameState>(window, mapPath, *this);
}