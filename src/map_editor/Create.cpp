#include "../../includes/map_editor/Create.hpp"
#include "../../includes/core/Debug.hpp"
#include "../../includes/lib/global_values.hpp"
#include "../../includes/lib/textures.hpp"

#include <iostream>

Create::Create(sf::RenderWindow &window) : window(window)
{
    if (!wallTex.loadFromFile(STRAIGHT_LINE_H))
    {
        std::cerr << "Error loading straight line texture" << std::endl;
    }

    if (!cornerTex.loadFromFile(ANGLE_0))
    {
        std::cerr << "Error loading angle texture" << std::endl;
    }

    if (!pacdotTex.loadFromFile(PACDOT_TEX))
    {
        std::cerr << "Error loading pacdot texture" << std::endl;
    }

    if (!ghostDoorTex.loadFromFile(GHOST_DOOR_TEX))
    {
        std::cerr << "Error loading ghost door texture" << std::endl;
    }

    if (!gameAsset.loadFromFile(ASSET))
    {
        std::cerr << "Error loading fruit texture" << std::endl;
    }

    cursorPos = {0, 0};

    float x = 0.f;
    float y = MAP_HEIGHT * TILE_SIZE;

    sf::Sprite pacdotSprite(pacdotTex);
    pacdotSprite.setScale({(float)TILE_SIZE / pacdotTex.getSize().x, (float)TILE_SIZE / pacdotTex.getSize().y});
    pacdotSprite.setPosition({x, y});
    tileSprites.push_back({"PACDOT", pacdotSprite, {x, y}});

    sf::Sprite wallSprite(wallTex);
    wallSprite.setScale({(float)TILE_SIZE / wallTex.getSize().x, (float)TILE_SIZE / wallTex.getSize().y});
    wallSprite.setPosition({x + (TILE_SIZE * 2), y});
    tileSprites.push_back({"WALL", wallSprite, {x + (TILE_SIZE * 2), y}});

    sf::Sprite cornerSprite(cornerTex);
    cornerSprite.setScale({(float)TILE_SIZE / cornerTex.getSize().x, (float)TILE_SIZE / cornerTex.getSize().y});
    cornerSprite.setPosition({x + (TILE_SIZE * 4), y});
    tileSprites.push_back({"CORNER", cornerSprite, {x + (TILE_SIZE * 4), y}});

    sf::Sprite powerPelletSprite(pacdotTex);
    powerPelletSprite.setScale({(float)TILE_SIZE / pacdotTex.getSize().x, (float)TILE_SIZE / pacdotTex.getSize().y});
    powerPelletSprite.setPosition({x + (TILE_SIZE * 6), y});
    tileSprites.push_back({"POWER PELLET", powerPelletSprite, {x + (TILE_SIZE * 6), y}});

    sf::Sprite ghostDoorSprite(ghostDoorTex);
    ghostDoorSprite.setScale({(float)TILE_SIZE / wallTex.getSize().x, (float)TILE_SIZE / wallTex.getSize().y});
    ghostDoorSprite.setPosition({x + (TILE_SIZE * 8), y});
    tileSprites.push_back({"GHOST DOOR", ghostDoorSprite, {x + (TILE_SIZE * 8), y}});

    // Cherry
    sf::IntRect texRect({2 * (TILE_SIZE / 2), 3 * (TILE_SIZE / 2)}, {TILE_SIZE / 2, TILE_SIZE / 2});
    sf::Sprite fruit(gameAsset, texRect);
    fruit.setScale({2.f, 2.f});
    fruit.setPosition({x + (TILE_SIZE * 10), y});
    tileSprites.push_back({"FRUIT GENERATOR", fruit, {x + (TILE_SIZE * 10), y}});

    // PacMan
    texRect = sf::IntRect({1 * (TILE_SIZE / 2), 1 * (TILE_SIZE / 2)}, {TILE_SIZE / 2, TILE_SIZE / 2});
    sf::Sprite pacmanSprite(gameAsset, texRect);
    pacmanSprite.setScale({2.f, 2.f});
    pacmanSprite.setPosition({x + (TILE_SIZE * 12), y});
    tileSprites.push_back({"PACMAN SPAWN", pacmanSprite, {x + (TILE_SIZE * 12), y}});

    texRect = sf::IntRect({0, 4 * (TILE_SIZE / 2)}, {TILE_SIZE / 2, TILE_SIZE / 2});
    sf::Sprite blinkySprite(gameAsset, texRect);
    blinkySprite.setScale({2.f, 2.f});
    blinkySprite.setPosition({x + (TILE_SIZE * 14), y});
    tileSprites.push_back({"BLINKY", blinkySprite, {x + (TILE_SIZE * 14), y}});

    texRect = sf::IntRect({0, 5 * (TILE_SIZE / 2)}, {TILE_SIZE / 2, TILE_SIZE / 2});
    sf::Sprite pinkySprite(gameAsset, texRect);
    pinkySprite.setScale({2.f, 2.f});
    pinkySprite.setPosition({x + (TILE_SIZE * 16), y});
    tileSprites.push_back({"PINKY", pinkySprite, {x + (TILE_SIZE * 16), y}});

    texRect = sf::IntRect({0, 6 * (TILE_SIZE / 2)}, {TILE_SIZE / 2, TILE_SIZE / 2});
    sf::Sprite inkySprite(gameAsset, texRect);
    inkySprite.setScale({2.f, 2.f});
    inkySprite.setPosition({x + (TILE_SIZE * 18), y});
    tileSprites.push_back({"INKY", inkySprite, {x + (TILE_SIZE * 18), y}});

    texRect = sf::IntRect({0, 7 * (TILE_SIZE / 2)}, {TILE_SIZE / 2, TILE_SIZE / 2});
    sf::Sprite clydeSprite(gameAsset, texRect);
    clydeSprite.setScale({2.f, 2.f});
    clydeSprite.setPosition({x + (TILE_SIZE * 20), y});
    tileSprites.push_back({"CLYDE", clydeSprite, {x + (TILE_SIZE * 20), y}});
}

void Create::doGraphics()
{
    Debug::drawGrid(window);
    drawCursor();
    doUI();
}

void Create::drawCursor()
{
    if (!isCursorOnMap())
    {
        return;
    }

    sf::RectangleShape cursor(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    cursor.setPosition({static_cast<float>(cursorPos.x) * TILE_SIZE, static_cast<float>(cursorPos.y) * TILE_SIZE});
    cursor.setFillColor(sf::Color::Transparent);
    cursor.setOutlineThickness(1.5f);
    cursor.setOutlineColor(sf::Color::Red);
    window.draw(cursor);
}

void Create::setCursorPos(int x, int y)
{
    cursorPos.x = x;
    cursorPos.y = y;
}

void Create::doUI()
{
    sf::RectangleShape outline(sf::Vector2f(TILE_SIZE * MAP_WIDTH, TILE_SIZE * 5));
    outline.setPosition({0.f, TILE_SIZE * MAP_HEIGHT});
    outline.setFillColor(sf::Color::Black);
    outline.setOutlineThickness(1.f);
    outline.setOutlineColor(Debug::gridColor);
    window.draw(outline);

    for (size_t i = 0; i < tileSprites.size(); ++i)
    {
        auto tile = tileSprites[i];
        tile.sprite.setScale(tile.sprite.getScale() * 2.f);

        window.draw(tile.sprite);
        sf::FloatRect bounds = tile.sprite.getGlobalBounds();

        if (bounds.contains({static_cast<float>(cursorPos.x) * TILE_SIZE, static_cast<float>(cursorPos.y) * TILE_SIZE}))
        {
            hoveredTileIndex = i;
        }
    }

    // Risolvere qui: il cursore all'inizio è in 0,0; se seleziono una tile del menu e mi sposto sulla mappa, il cursore (nel menu) non viene disegnato
    if ((hoveredTileIndex == -1 || isCursorOnMap()) && selectedTileIndex != -1)
    {
        hoveredTileIndex = -1;
        return;
    }

    sf::RectangleShape selectedTileOutline(sf::Vector2f(TILE_SIZE * 2, TILE_SIZE * 2));
    selectedTileOutline.setFillColor(sf::Color::Transparent);
    selectedTileOutline.setOutlineThickness(2.f);
    selectedTileOutline.setOutlineColor(sf::Color::Yellow);
    std::string selectedTileName = "SELECTED TILE";

    if (selectedTileIndex == -1)
    {
        const Tile &selectedTile = tileSprites[hoveredTileIndex];
        selectedTileOutline.setPosition(selectedTile.position);
        selectedTileName = selectedTile.name;
    }
    else
    {
        const Tile &selectedTile = tileSprites[selectedTileIndex];
        selectedTileOutline.setPosition(selectedTile.position);
        selectedTileName = selectedTile.name;
    }

    window.draw(selectedTileOutline);

    arcadeText.drawString(selectedTileName, 0, MAP_HEIGHT + 3, window, TextColor::WHITE);
}

bool Create::isCursorOnMap()
{
    return cursorPos.x >= 0 && cursorPos.x < MAP_WIDTH && cursorPos.y >= 0 && cursorPos.y < MAP_HEIGHT;
}

void Create::handle(const sf::Event::MouseButtonPressed &mouseButton)
{
    if (mouseButton.button == sf::Mouse::Button::Left)
    {
        if (isCursorOnMap())
        {
            std::cout << "Clicked on map tile" << std::endl;
        }
        else if (hoveredTileIndex != -1)
        {
            selectedTileIndex = hoveredTileIndex;
        }
    }
}