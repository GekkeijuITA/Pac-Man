#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "../includes/text_ui.hpp"
#include "../includes/textures.hpp"
#include "../includes/global_values.hpp"

inline const int FIRST_OPTION = 6;
inline const int LAST_OPTION = 10;

struct GameMenu
{
    sf::Texture tex;
    sf::View view;
    sf::Vector2i cursorPosition = {10, FIRST_OPTION};
    ArcadeText arcadeText;
    std::string title;
    std::vector<std::string> options;

    GameMenu();
    GameMenu(sf::View v, std::string title, std::vector<std::string> options);

public:
    void draw(sf::RenderWindow &window);
    void setCursorPosition(int x, int y);
    sf::Vector2i getCursorPosition();
};