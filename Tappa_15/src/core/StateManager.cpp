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

void handle(const sf::Event::MouseMoved &mouseMoved, StateManager &sm)
{
    switch (sm.currentMode)
    {
    case StateManager::MAIN_MENU:
        sm.mainMenuState->handle(mouseMoved);
        break;
    case StateManager::NORMAL_GAME:
        sm.gameState->handle(mouseMoved);
        break;
    case StateManager::MAP_EDITOR:
        sm.mapEditor->handle(mouseMoved);
        break;
    case StateManager::SCENARIO_EDITOR:
        sm.scenarioEditorState->handle(mouseMoved);
        break;
    }
}

void handle(const sf::Event::MouseButtonPressed &mouseButton, StateManager &sm)
{
    switch (sm.currentMode)
    {
    case StateManager::MAIN_MENU:
        sm.mainMenuState->handle(mouseButton);
        break;
    case StateManager::NORMAL_GAME:
        sm.gameState->handle(mouseButton);
        break;
    case StateManager::MAP_EDITOR:
        sm.mapEditor->handle(mouseButton);
        break;
    case StateManager::SCENARIO_EDITOR:
        sm.scenarioEditorState->handle(mouseButton);
        break;
    }
}

void handle(const sf::Event::MouseWheelScrolled &mouseWheel, StateManager &sm)
{
    switch (sm.currentMode)
    {
    case StateManager::NORMAL_GAME:
        sm.gameState->handle(mouseWheel);
        break;
    case StateManager::MAP_EDITOR:
        sm.mapEditor->handle(mouseWheel);
        break;
    case StateManager::SCENARIO_EDITOR:
        sm.scenarioEditorState->handle(mouseWheel);
        break;
    }
}

void handle(const sf::Event::Closed &close, StateManager &sm, GameState &gs)
{
    sm.window.close();
}

void handle(const sf::Event::TextEntered &textEntered, StateManager &sm)
{
    if(sm.currentMode == StateManager::MAP_EDITOR) {
        if(sm.mapEditor->currentMode == sm.mapEditor->CREATE && sm.mapEditor->create->writingNameMap) {
            sm.mapEditor->create->handle(textEntered);
        }
    }
    else if(sm.currentMode == StateManager::SCENARIO_EDITOR) {
        if(sm.scenarioEditorState->currentMode == sm.scenarioEditorState->SAVE_PROMPT) {
            sm.scenarioEditorState->handle(textEntered);
        }
    }   
}

template <typename T>
void handle(const T &, StateManager &sm, GameState &gs)
{
    // eventi non gestiti
}

template <typename T>
void handle(const T &, StateManager &sm)
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
                        { handle(event, *this, *gameState); handle(event, *this); });
}
