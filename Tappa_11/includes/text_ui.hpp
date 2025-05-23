#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

#include "../includes/textures.hpp"
#include "../includes/global_values.hpp"

inline const sf::Vector2i CHAR_A = {0, 0};
inline const sf::Vector2i CHAR_B = {1, 0};
inline const sf::Vector2i CHAR_C = {2, 0};
inline const sf::Vector2i CHAR_D = {3, 0};
inline const sf::Vector2i CHAR_E = {4, 0};
inline const sf::Vector2i CHAR_F = {5, 0};
inline const sf::Vector2i CHAR_G = {6, 0};
inline const sf::Vector2i CHAR_H = {7, 0};
inline const sf::Vector2i CHAR_I = {8, 0};
inline const sf::Vector2i CHAR_J = {9, 0};
inline const sf::Vector2i CHAR_K = {10, 0};
inline const sf::Vector2i CHAR_L = {11, 0};
inline const sf::Vector2i CHAR_M = {12, 0};
inline const sf::Vector2i CHAR_N = {13, 0};
inline const sf::Vector2i CHAR_O = {14, 0};
inline const sf::Vector2i CHAR_P = {0, 1};
inline const sf::Vector2i CHAR_Q = {1, 1};
inline const sf::Vector2i CHAR_R = {2, 1};
inline const sf::Vector2i CHAR_S = {3, 1};
inline const sf::Vector2i CHAR_T = {4, 1};
inline const sf::Vector2i CHAR_U = {5, 1};
inline const sf::Vector2i CHAR_V = {6, 1};
inline const sf::Vector2i CHAR_W = {7, 1};
inline const sf::Vector2i CHAR_X = {8, 1};
inline const sf::Vector2i CHAR_Y = {9, 1};
inline const sf::Vector2i CHAR_Z = {10, 1};
inline const sf::Vector2i CHAR_EXCLAMATION_MARK = {11, 1};
inline const sf::Vector2i CHAR_COPYRIGHT = {12, 1};
inline const sf::Vector2i CHAR_0 = {0, 2};
inline const sf::Vector2i CHAR_1 = {1, 2};
inline const sf::Vector2i CHAR_2 = {2, 2};
inline const sf::Vector2i CHAR_3 = {3, 2};
inline const sf::Vector2i CHAR_4 = {4, 2};
inline const sf::Vector2i CHAR_5 = {5, 2};
inline const sf::Vector2i CHAR_6 = {6, 2};
inline const sf::Vector2i CHAR_7 = {7, 2};
inline const sf::Vector2i CHAR_8 = {8, 2};
inline const sf::Vector2i CHAR_9 = {9, 2};
inline const sf::Vector2i CHAR_SLASH = {10, 2};
inline const sf::Vector2i CHAR_MINUS = {11, 2};
inline const sf::Vector2i CHAR_QUOTE = {12, 2};

inline const std::map<std::string, sf::Vector2i> CHAR_MAP = {
    {"A", CHAR_A},
    {"B", CHAR_B},
    {"C", CHAR_C},
    {"D", CHAR_D},
    {"E", CHAR_E},
    {"F", CHAR_F},
    {"G", CHAR_G},
    {"H", CHAR_H},
    {"I", CHAR_I},
    {"J", CHAR_J},
    {"K", CHAR_K},
    {"L", CHAR_L},
    {"M", CHAR_M},
    {"N", CHAR_N},
    {"O", CHAR_O},
    {"P", CHAR_P},
    {"Q", CHAR_Q},
    {"R", CHAR_R},
    {"S", CHAR_S},
    {"T", CHAR_T},
    {"U", CHAR_U},
    {"V", CHAR_V},
    {"W", CHAR_W},
    {"X", CHAR_X},
    {"Y", CHAR_Y},
    {"Z", CHAR_Z},
    {"!", CHAR_EXCLAMATION_MARK},
    {"CPYRGHT", CHAR_COPYRIGHT},
    {"0", CHAR_0},
    {"1", CHAR_1},
    {"2", CHAR_2},
    {"3", CHAR_3},
    {"4", CHAR_4},
    {"5", CHAR_5},
    {"6", CHAR_6},
    {"7", CHAR_7},
    {"8", CHAR_8},
    {"9", CHAR_9},
    {"/", CHAR_SLASH},
    {"-", CHAR_MINUS},
    {"'", CHAR_QUOTE}
};

struct ArcadeText
{
    sf::Texture tex;
    std::string text;

    ArcadeText();

public:
    void drawChar(float x, float y, sf::Vector2i charPos, sf::RenderWindow &window, float scaleFactor = 1.f, float i = 0);
    
    void drawString(std::string str, float x, float y, sf::RenderWindow &window);
    void drawString(std::string str, float x, float y, sf::RenderWindow &window, sf::Vector2i offset);
    void drawString(std::string str, float x, float y, sf::RenderWindow &window, float scaleFactor);
    void drawString(std::string str, float x, float y, sf::RenderWindow &window, float scaleFactor, sf::Vector2i offset);
};