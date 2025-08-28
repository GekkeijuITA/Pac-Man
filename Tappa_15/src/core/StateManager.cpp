#include "../../includes/core/StateManager.hpp"

void handle(const sf::Event::KeyPressed &key, StateManager &sm, GameState &gs)
{
    switch (sm.currentMode)
    {
    case StateManager::NORMAL_GAME:
    {
        if (gs.currentMode == GameState::MENU)
        {
            gs.mainMenu.handle(key);
        }
        else if (gs.currentMode == GameState::PLAY_MAP)
        {
            gs.levelSelectorState->handle(key);
        }
        else if (gs.currentMode == GameState::PLAY_SCENARIO)
        {
            gs.scenariosList.handle(key);
        }
        else
            gs.handle(key);
        break;
    }
    case StateManager::MAIN_MENU:
    {
        sm.mainMenuState->menu.handle(key);
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
    case StateManager::SCENARIO_EDITOR:
    {
        if (sm.scenarioEditorState->currentMode == ScenarioEditor::CREATE)
        {
            sm.scenarioEditorState->levelSelectorState->handle(key);
        }
        else if (sm.scenarioEditorState->currentMode == ScenarioEditor::EDIT)
        {
            sm.scenarioEditorState->scenariosList.handle(key);
        }
        else if (sm.scenarioEditorState->currentMode == ScenarioEditor::SAVE_PROMPT)
        {
            sm.scenarioEditorState->handle(key);
        }
        else
        {
            sm.scenarioEditorState->menu.handle(key);
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

void handle(const sf::Event::MouseButtonPressed &mousePressed, ScenarioEditor &se)
{
    if (se.currentMode == se.SAVE_PROMPT)
    {
        se.handle(mousePressed);
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

void handle(const sf::Event::TextEntered &textEntered, ScenarioEditor &se)
{
    if (se.currentMode == se.SAVE_PROMPT)
    {
        se.handle(textEntered);
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

template <typename T>
void handle(const T &, ScenarioEditor &se)
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
    mainMenuState = std::make_unique<MainMenuState>(window, *this);
    levelSelectorState = std::make_unique<LevelSelectorState>(window, *this, *mapEditor);
    scenarioEditorState = std::make_unique<ScenarioEditor>(window, *this);
    gameState = std::make_unique<GameState>(window, *this);
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
        break;
    case MAP_EDITOR:
        mapEditor->doGraphics();
        break;
    case SCENARIO_EDITOR:
        scenarioEditorState->doGraphics();
        break;
    }
}

void StateManager::handleInputs()
{
    window.handleEvents([&](const auto &event)
                        { handle(event, *this, *gameState); handle(event, *mapEditor); 
                          handle(event, *scenarioEditorState); });
}
