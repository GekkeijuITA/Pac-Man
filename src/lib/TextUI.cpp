#include "../../includes/lib/text_ui.hpp"
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
void ArcadeText::drawChar(float x, float y, sf::Vector2i charPos, sf::RenderWindow &window, float scaleFactor, float i)
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
void ArcadeText::drawString(std::string str, float x, float y, sf::RenderWindow &window, float scaleFactor, TextColor color)
{
    float offset = 0.f;
    for (size_t i = 0; i < str.length(); i++)
    {
        offset = i;
        std::string c(1, std::toupper(str[i]));
        if (c == "\n")
        {
            y += 1.f;
            offset = 0.f;
            continue;
        }
        else if (c == " " || CHAR_MAP.find(c) == CHAR_MAP.end())
        {
            offset++;
            continue;
        }
        drawChar(x, y, CHAR_MAP.at(c) + getColor(color), window, scaleFactor, offset);
        offset++;
    }
}

// Disegna la stringa con un offset per lo sprite sheet
void ArcadeText::drawString(std::string str, float x, float y, sf::RenderWindow &window, TextColor color)
{
    float offset = 0.f;
    for (size_t i = 0; i < str.length(); i++)
    {
        std::string c(1, std::toupper(str[i]));
        if (c == "\n")
        {
            y += 1.f;
            offset = 0.f;
            continue;
        }
        else if (c == " " || CHAR_MAP.find(c) == CHAR_MAP.end())
        {
            offset++;
            continue;
        }

        drawChar(x, y, CHAR_MAP.at(c) + getColor(color), window, 1.f, offset);
        offset++;
    }
}

// Disegna la stringa con un fattore di scala
void ArcadeText::drawString(std::string str, float x, float y, sf::RenderWindow &window, float scaleFactor)
{
    float offset = 0.f;
    for (size_t i = 0; i < str.length(); i++)
    {
        offset = i;
        std::string c(1, std::toupper(str[i]));
        if (c == "\n")
        {
            y += 1.f;
            offset = 0.f;
            continue;
        }
        else if (c == " " || CHAR_MAP.find(c) == CHAR_MAP.end())
        {
            offset++;
            continue;
        }
        drawChar(x, y, CHAR_MAP.at(c), window, scaleFactor, offset);
        offset++;
    }
}

// Disegna la stringa senza fattore di scala e offset
void ArcadeText::drawString(std::string str, float x, float y, sf::RenderWindow &window)
{
    float offset = 0.f;
    for (size_t i = 0; i < str.length(); i++)
    {
        offset = i;
        std::string c(1, std::toupper(str[i]));
        if (c == "\n")
        {
            y += 1.f;
            offset = 0.f;
            continue;
        }
        else if (c == " " || CHAR_MAP.find(c) == CHAR_MAP.end())
        {
            offset++;
            continue;
        }
        drawChar(x, y, CHAR_MAP.at(c), window, 1.f, offset);
        offset++;
    }
}

sf::Vector2i ArcadeText::getColor(TextColor color)
{
    switch (color)
    {
    case RED:
        return {0, 4};
    case PINK:
        return {0, 8};
    case CYAN:
        return {0, 12};
    case ORANGE:
        return {0, 16};
    case SALMON:
        return {0, 20};
    case YELLOW:
        return {0, 24};
    default:
        return {0, 0};
    }
}