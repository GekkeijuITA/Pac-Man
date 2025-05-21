#include "../../includes/map_editor/MapEditor.hpp"
#include "../../includes/core/Debug.hpp"

MapEditor::MapEditor(sf::RenderWindow &window, StateManager &sm) : window(window), stateManager(sm)
{
    options = {
        {"CREATE", [this]()
         { currentMode = CREATE; }},
        {"EDIT", [this]()
         { currentMode = EDIT; }},
        {"BACK", [this]()
         {
             stateManager.currentMode = StateManager::MAIN_MENU;
             stateManager.mainMenuState->getHighscore();
             currentMode = MENU;
         }}};

    menu = GameMenu(window.getView(), "MAP EDITOR", {4, 1}, TextColor::WHITE, options, {9, 9});

    create = std::make_unique<Create>(window);
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
        std::cout << "EDIT MODE" << std::endl;
        break;
    }
    default:
    {
        std::cerr << "Unknown mode" << std::endl;
        break;
    }
    }
    // Debug::drawGrid(window);
}