#include "../includes/text_ui.hpp"
#include <iostream>

ArcadeText::ArcadeText()
{
    if (!tex.loadFromFile(TEXT))
    {
        std::cerr << "Error loading texture" << std::endl;
        exit(1);
    }
}

// Disegna un carattere in una posizione specifica
void ArcadeText::drawChar(int x, int y, sf::Vector2i charPos, sf::RenderWindow &window, float scaleFactor, int i)
{
    sf::Sprite textSprite = createSprite(
        tex,
        charPos,
        {(float)TILE_SIZE / TEXT_SIZE, (float)TILE_SIZE / TEXT_SIZE},
        scaleFactor,
        TEXT_SIZE,
        false);

    textSprite.setPosition({(x * TILE_SIZE) + ((scaleFactor * i) * TILE_SIZE), (float)y * TILE_SIZE});
    window.draw(textSprite);
}

// Disegna la stringa con un fattore di scala e un offset per lo sprite sheet
void ArcadeText::drawString(std::string str, int x, int y, sf::RenderWindow &window, float scaleFactor, sf::Vector2i offset)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        std::string c(1, str[i]);
        if (c == " ")
            continue;
        drawChar(x, y, CHAR_MAP.at(c) + offset, window, scaleFactor, i);
    }
}

// Disegna la stringa con un offset per lo sprite sheet
void ArcadeText::drawString(std::string str, int x, int y, sf::RenderWindow &window, sf::Vector2i offset)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        std::string c(1, str[i]);
        if (c == " " || CHAR_MAP.find(c) == CHAR_MAP.end())
            continue;

        drawChar(x, y, CHAR_MAP.at(c) + offset, window, 1.f, i);
    }
}

// Disegna la stringa con un fattore di scala
void ArcadeText::drawString(std::string str, int x, int y, sf::RenderWindow &window, float scaleFactor)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        std::string c(1, str[i]);
        if (c == " ")
            continue;
        drawChar(x, y, CHAR_MAP.at(c), window, scaleFactor, i);
    }
}

// Disegna la stringa senza fattore di scala e offset
void ArcadeText::drawString(std::string str, int x, int y, sf::RenderWindow &window)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        std::string c(1, str[i]);
        if (c == " ")
            continue;
        drawChar(x, y, CHAR_MAP.at(c), window, 1.f, i);
    }
}