#include "../../includes/map_editor/MapEditor.hpp"
#include "../../includes/core/Debug.hpp"

MapEditor::MapEditor(sf::RenderWindow &window, StateManager &sm) : window(window), stateManager(sm)
{
    options = {
        {"CREATE", [this]()
         {
             currentMode = CREATE;
         }},
        {"EDIT", [this]()
         { currentMode = EDIT; }},
        {"BACK", [this]()
         {
             stateManager.currentMode = StateManager::MAIN_MENU;
             stateManager.mainMenuState->getHighscore();
             currentMode = MENU;
         }}};

    menu = GameMenu(window.getView(), "MAP EDITOR", {4, 1}, TextColor::WHITE, options, {9, 9});
    levelSelectorState = std::make_unique<LevelSelectorState>(window, stateManager, *this);
    create = std::make_unique<Create>(window, *this);
}

void MapEditor::doGraphics()
{
    switch (currentMode)
    {
    case MENU:
    {
        menu.draw(window);
        break;
    }
    case CREATE:
    {
        create->doGraphics();
        break;
    }
    case EDIT:
    {
        levelSelectorState->draw();
        break;
    }
    default:
    {
        std::cerr << "Unknown mode" << std::endl;
        break;
    }
    }
}

void MapEditor::initEditor(std::string mapName)
{
    currentMode = CREATE;
    create = std::make_unique<Create>(window, *this, mapName);
}