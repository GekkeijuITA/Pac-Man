#pragma once
#include <SFML/Graphics.hpp>
#include "../game_elements/GameState.hpp"
#include "../MainMenuState.hpp"
#include "../LevelSelectorState.hpp"
#include "../map_editor/MapEditor.hpp"
#include "../scenario_editor/ScenarioEditor.hpp"
#include <memory>

struct GameState;
struct MainMenuState;
struct LevelSelectorState;
struct MapEditor;
struct ScenarioEditor;

struct StateManager
{
    enum Mode
    {
        MAIN_MENU,
        NORMAL_GAME,
        MAP_EDITOR,
        SCENARIO_EDITOR
    };

    Mode currentMode = MAIN_MENU;
    std::unique_ptr<GameState> gameState;
    std::unique_ptr<MainMenuState> mainMenuState;
    std::unique_ptr<LevelSelectorState> levelSelectorState;
    std::unique_ptr<MapEditor> mapEditor;
    std::unique_ptr<ScenarioEditor> scenarioEditorState;

    sf::RenderWindow window;
    unsigned w, h;
    std::string title;

    StateManager(unsigned w, unsigned h, std::string title);

public:
    void update(float elapsed);
    void doGraphics();
    void handleInputs();
};