#include "../includes/GameMenu.hpp"
#include "../includes/lib/SoundManager.hpp"

#include <iostream>

GameMenu::GameMenu() {}

GameMenu::GameMenu(sf::View v, std::string title, sf::Vector2i titlePos, TextColor textColor, std::vector<MenuOption> options, sf::Vector2i startOptionsPos) : view(v),
                                                                                                                                                               title(title),
                                                                                                                                                               options(options),
                                                                                                                                                               titlePos(titlePos),
                                                                                                                                                               startOptionsPos(startOptionsPos),
                                                                                                                                                               textColor(textColor)
{
    if (!tex.loadFromFile(TEXT))
    {
        std::cerr << "Error loading texture" << std::endl;
        exit(1);
    }

    ArcadeText arcadeText;
    setCursorPosition(startOptionsPos.x - 1, startOptionsPos.y);
}

GameMenu::GameMenu(sf::View v, std::string title, sf::Vector2i titlePos, TextColor textColor, float scaleFactor, std::vector<MenuOption> options, sf::Vector2i startOptionsPos) : view(v),
                                                                                                                                                                                  title(title),
                                                                                                                                                                                  options(options),
                                                                                                                                                                                  titlePos(titlePos),
                                                                                                                                                                                  startOptionsPos(startOptionsPos),
                                                                                                                                                                                  textColor(textColor),
                                                                                                                                                                                  scaleFactor(scaleFactor)
{
    if (!tex.loadFromFile(TEXT))
    {
        std::cerr << "Error loading texture" << std::endl;
        exit(1);
    }

    ArcadeText arcadeText;
    setCursorPosition(startOptionsPos.x - 1, startOptionsPos.y);
}

void GameMenu::draw(sf::RenderWindow &window)
{
    sf::RectangleShape background(view.getSize());
    background.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(background);

    arcadeText.drawString(title, titlePos.x, titlePos.y, window, scaleFactor, textColor);
    for (auto &option : options)
    {
        arcadeText.drawString(option.name, startOptionsPos.x, startOptionsPos.y + (2 * (&option - &options[0])), window, TextColor::WHITE);
    }

    sf::CircleShape triangle(TILE_SIZE / 2.f, 3);
    triangle.setFillColor(sf::Color::Red);
    triangle.setPosition({TILE_SIZE * (cursorPosition.x + .5f), TILE_SIZE * ((cursorPosition.y + .5f))});
    triangle.setOrigin({TILE_SIZE / 2.f, TILE_SIZE / 2.f});
    triangle.setRotation(sf::degrees(90));
    window.draw(triangle);
}

void GameMenu::setCursorPosition(int x, int y)
{
    cursorPosition = {x, y};
}

sf::Vector2i GameMenu::getCursorPosition()
{
    return cursorPosition;
}

int GameMenu::getOptionsSize()
{
    return options.size();
}

// Muove il cursore verso l'alto
void GameMenu::moveCursorUp()
{
    if (cursorIndex > 0)
    {
        cursorIndex--;
    }
    else
    {
        cursorIndex = getOptionsSize() - 1;
    }

    setCursorPosition(cursorPosition.x, startOptionsPos.y + 2 * cursorIndex);
}

// Muove il cursore verso il basso
void GameMenu::moveCursorDown()
{
    if (cursorIndex < getOptionsSize() - 1)
    {
        cursorIndex++;
    }
    else
    {
        cursorIndex = 0;
    }

    setCursorPosition(cursorPosition.x, startOptionsPos.y + 2 * cursorIndex);
}

// Esegue l'azione associata all'opzione selezionata
void GameMenu::executeOption()
{
    options[cursorIndex].action();
}

void GameMenu::resetCursor()
{
    cursorIndex = 0;
    setCursorPosition(cursorPosition.x, startOptionsPos.y);
}

void GameMenu::handle(const sf::Event::KeyPressed &key)
{
    switch (key.scancode)
    {
    case sf::Keyboard::Scancode::Up:
        moveCursorUp();
        break;
    case sf::Keyboard::Scancode::Down:
        moveCursorDown();
        break;
    case sf::Keyboard::Scancode::Enter:
        SoundManager::getInstance().playSound("credit");
        executeOption();
        break;
    default:
        break;
    }
}