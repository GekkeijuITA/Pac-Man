#include "../includes/MainMenuState.hpp"
#include "../includes/core/Debug.hpp"
#include <fstream>

MainMenuState::MainMenuState(sf::RenderWindow &window, StateManager &sm, MapEditor &me)
    : window(window), stateManager(sm), mapEditor(me)
{
    getHighscore();

    options = {{"PLAY", [this]()
                { stateManager.currentMode = StateManager::LEVEL_SELECTOR; stateManager.levelSelectorState->loadMaps(); }},
               {"MAP EDITOR", [this]()
                { stateManager.currentMode = StateManager::MAP_EDITOR; }},
               {"QUIT", [this]()
                { stateManager.window.close(); }}};
    menu = GameMenu(window.getView(), "PAC-MAN", sf::Vector2i(3, 4), TextColor::YELLOW, 3.f, options, sf::Vector2i(11, 9));
}

void MainMenuState::draw()
{
    menu.draw(window);

    arcadeText.drawString("1UP", 3, 1, window, TextColor::RED);
    arcadeText.drawString("00", 6, 2, window, TextColor::WHITE);
    arcadeText.drawString("HIGH SCORE", 9, 1, window, TextColor::RED);
    drawScore(16, 2, highscore);
    // Debug::drawGrid(window);
}

void MainMenuState::drawScore(int x, int y, int score)
{
    std::string scoreString = std::to_string(score);
    int scoreLength = scoreString.length();
    int startX = x - scoreLength;

    arcadeText.drawString(scoreString, startX, y, window, TextColor::WHITE);
}

void MainMenuState::getHighscore()
{
    std::ifstream highscoreFile;
    highscoreFile.open("../Tappa_12/resources/highscore.txt", std::ios::in);
    if (!highscoreFile.is_open())
    {
        std::cerr << "Errore nell'apertura del file contenente l'highscore" << std::endl;
        return;
    }
    std::string highscoreString;
    std::getline(highscoreFile, highscoreString);
    highscore = highscoreString.empty() ? 0 : std::stoi(highscoreString);
    highscoreFile.close();
}