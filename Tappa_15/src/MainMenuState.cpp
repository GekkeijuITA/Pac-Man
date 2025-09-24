#include "../includes/MainMenuState.hpp"
#include "../includes/lib/Debug.hpp"
#include <fstream>

MainMenuState::MainMenuState(sf::RenderWindow &window, StateManager &sm)
    : window(window), stateManager(sm), options({{"PLAY", [this]()
                                                  { stateManager.currentMode = StateManager::NORMAL_GAME; stateManager.gameState->initScenarioList(); }},
                                                 {"MAP EDITOR", [this]()
                                                  { stateManager.currentMode = StateManager::MAP_EDITOR; }},
                                                 {"SCENARIO EDITOR", [this]()
                                                  { stateManager.currentMode = StateManager::SCENARIO_EDITOR; }},
                                                 {"QUIT", [this]()
                                                  { stateManager.window.close(); }}}),
      menu("PAC-MAN", sf::Vector2i(3, 4), TextColor::YELLOW, 3.f, options, sf::Vector2i(11, 9), window)
{
    getHighscore();
}

void MainMenuState::draw()
{
    menu.draw();

    arcadeText.drawString("1UP", 3, 1, window, TextColor::RED);
    arcadeText.drawString("00", 6, 2, window, TextColor::WHITE);
    arcadeText.drawString("HIGH SCORE", 9, 1, window, TextColor::RED);
    drawScore(16, 2, highscore);
    arcadeText.drawString("1. Use the arrow keys\nto move", 4, 20, window, TextColor::SALMON);
    arcadeText.drawString("2. Press 'ENTER' to\nselect", 4, 24, window, TextColor::SALMON);
    arcadeText.drawString("3. press 'ESC' to pause", 4, 28, window, TextColor::SALMON);
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
    highscoreFile.open("../../Tappa_15/resources/highscore.txt", std::ios::in);
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

void MainMenuState::handle(const sf::Event::MouseButtonPressed &mouse)
{
    menu.handle(mouse);
}

void MainMenuState::handle(const sf::Event::MouseMoved &mouse)
{
    menu.handle(mouse);
}