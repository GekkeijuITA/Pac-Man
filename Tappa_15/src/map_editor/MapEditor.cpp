#include "../../includes/map_editor/MapEditor.hpp"

MapEditor::MapEditor(sf::RenderWindow &window, StateManager &sm) : window(window), stateManager(sm), options({{"CREATE", [this]()
                                                                                                               {
                                                                                                                   currentMode = CREATE;
                                                                                                               }},
                                                                                                              {"EDIT", [this]()
                                                                                                               { currentMode = EDIT; levelSelectorState->loadMaps(); }},
                                                                                                              {"BACK", [this]()
                                                                                                               {
                                                                                                                   stateManager.currentMode = StateManager::MAIN_MENU;
                                                                                                                   stateManager.mainMenuState->getHighscore();
                                                                                                                   currentMode = MENU;
                                                                                                               }}}),
                                                                   menu("MAP EDITOR", {4, 1}, TextColor::WHITE, options, {9, 9}, window)
{
    levelSelectorState = std::make_unique<LevelSelectorState>(window, stateManager, *this);
    create = std::make_unique<Create>(window, *this);
}

void MapEditor::doGraphics()
{
    switch (currentMode)
    {
    case MENU:
    {
        menu.draw();
        arcadeText.drawString("1. select a tile to\nplace on the map with\nthe mouse", 4, 20, window, TextColor::SALMON);
        arcadeText.drawString("2. click to place the\ntile on the map", 4, 24, window, TextColor::SALMON);
        arcadeText.drawString("3. press 'ESC' to open\nthe options menu", 4, 27, window, TextColor::SALMON);
        arcadeText.drawString("4. while on a tile\npress 'R' to rotate it", 4, 30, window, TextColor::SALMON);
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

void MapEditor::handle(const sf::Event::MouseButtonPressed &mouse)
{
    if (currentMode == CREATE)
    {
        create->handle(mouse);
    }
    else if (currentMode == MENU)
    {
        menu.handle(mouse);
    }
    else if (currentMode == EDIT)
    {
        levelSelectorState->handle(mouse);
    }
}

void MapEditor::handle(const sf::Event::MouseMoved &mouse)
{
    if (currentMode == CREATE)
    {
        create->handle(mouse);
    }
    else if (currentMode == MENU)
    {
        menu.handle(mouse);
    }
    else if (currentMode == EDIT)
    {
        levelSelectorState->handle(mouse);
    }
}

void MapEditor::handle(const sf::Event::MouseWheelScrolled &wheel)
{
    if (currentMode == EDIT)
    {
        levelSelectorState->handle(wheel);
    }
}