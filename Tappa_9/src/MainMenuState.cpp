#include "../includes/MainMenuState.hpp"
#include "../includes/Debug.hpp"
#include <fstream>

MainMenuState::MainMenuState(sf::RenderWindow &window, StateManager &stateManager)
    : window(window), stateManager(stateManager)
{
    getHighscore();
}

void MainMenuState::draw()
{
    arcadeText.drawString("1UP", 3, 1, window, sf::Vector2i(0, 4));
    arcadeText.drawString("00", 6, 2, window);
    arcadeText.drawString("HIGH SCORE", 9, 1, window, sf::Vector2i(0, 4));
    drawScore(16, 2, highscore);
    arcadeText.drawString("PAC-MAN", 3, 4, window, 3.f, sf::Vector2i(0, 24));

    for (const auto &option : options)
    {
        arcadeText.drawString(option, 11, 9 + (&option - &options[0]) * 2, window);
    }

    sf::CircleShape triangle(TILE_SIZE / 2.f, 3);
    triangle.setFillColor(sf::Color::Red);
    triangle.setPosition({TILE_SIZE * (cursorPosition.x + .5f), TILE_SIZE * ((cursorPosition.y + (cursorIndex * 2)) + .5f)});
    triangle.setOrigin({TILE_SIZE / 2.f, TILE_SIZE / 2.f});
    triangle.setRotation(sf::degrees(90));
    window.draw(triangle);

    // Debug::drawGrid(window);
}

void MainMenuState::drawScore(int x, int y, int score)
{
    std::string scoreString = std::to_string(score);
    int scoreLength = scoreString.length();
    int startX = x - scoreLength;

    arcadeText.drawString(scoreString, startX, y, window);
}

void MainMenuState::getHighscore()
{
    std::ifstream highscoreFile;
    highscoreFile.open("../Tappa_9/resources/highscore.txt", std::ios::in);
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