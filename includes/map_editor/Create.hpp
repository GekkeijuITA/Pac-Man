#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include "../../includes/lib/text_ui.hpp"

struct Tile
{
    std::string name;
    sf::Sprite sprite;
    sf::Vector2f position;
};

struct Create
{
    sf::RenderWindow &window;
    sf::Vector2i cursorPos;
    sf::Texture wallTex, cornerTex, pacdotTex, ghostDoorTex, gameAsset;
    std::vector<Tile> tileSprites;
    int hoveredTileIndex = -1, selectedTileIndex = -1;
    ArcadeText arcadeText;

    Create(sf::RenderWindow &window);

public:
    void doGraphics();
    void doUI();
    void setCursorPos(int x, int y);
    void handle(const sf::Event::MouseButtonPressed &mouseButton);

private:
    void drawCursor();
    bool isCursorOnMap();
};