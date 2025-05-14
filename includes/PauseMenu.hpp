#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "../includes/text_ui.hpp"
#include "../includes/textures.hpp"
#include "../includes/global_values.hpp"

inline const int FIRST_OPTION = 6;
inline const int LAST_OPTION = 10;

struct PauseMenu
{
    sf::Texture tex;
    sf::View view;
    sf::Vector2i cursorPosition = {10, FIRST_OPTION};

    PauseMenu();
    PauseMenu(sf::View v);

public:
    void draw(sf::RenderWindow &window);
    void setCursorPosition(int x, int y);
    sf::Vector2i getCursorPosition();

private:
    void drawChar(int x, int y, sf::Vector2i charPos, sf::RenderWindow &window, float scaleFactor = 1.f, int i = 0);
    void drawString(std::string str, int x, int y, sf::RenderWindow &window, float scaleFactor = 1.f);
};