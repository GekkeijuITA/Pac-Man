#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "../GameMenu.hpp"
#include "../core/StateManager.hpp"
#include "../LevelSelectorState.hpp"

struct StateManager;
struct LevelSelectorState;

struct ScenarioEditor
{
    enum Mode
    {
        MENU,
        CREATE,
        EDIT,
        DELETE,
        SAVE_PROMPT
    };

    Mode currentMode = MENU;
    sf::RenderWindow &window;

    std::vector<MenuOption> options = {}, scenarios = {};
    GameMenu menu, scenariosList;
    
    StateManager &stateManager;

    ArcadeText arcadeText;
    int cursorIndex = 0;

    std::unique_ptr<LevelSelectorState> levelSelectorState;

    ScenarioEditor(sf::RenderWindow &window, StateManager &sm);

    std::vector<std::string> scenario_list;
    std::string scenario_name;
    bool loopScenario = false;
    bool writingScenarioName = false;
    int textCursorPos = 0;

    float promptWidth = 600.f;
    float promptHeight = 200.f;
    float saveTextX, saveTextY;
    float optionY, yesTextX, noTextX;

private:
    void
    initScenarioList();
    void loadScenario(const std::string &scenarioName);

public:
    void doGraphics();
    void addToScenario(std::string mapName);
    void removeFromScenario(std::string mapName);
    bool isInScenario(const std::string &mapName);
    void drawSavePrompt();
    void saveScenario(const std::string &scenarioName);
    std::string defaultScenarioName();
    static int getScenarioSize(std::string &scenarioName);

    void handle(const sf::Event::KeyPressed &key);
    void handle(const sf::Event::TextEntered &textEntered);
    void handle(const sf::Event::MouseButtonPressed &mousePressed);
    void handle(const sf::Event::MouseMoved &mouseMoved);
};