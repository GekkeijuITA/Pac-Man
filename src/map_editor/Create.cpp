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

    if (!ghostDoorTex.loadFromFile(GHOST_DOOR_TEX_H))
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
    tileSprites.insert({PACDOT, {"PACDOT", pacdotSprite, {x, y}}});
    tileDisplayOrder.push_back(PACDOT);

    sf::Sprite wallSprite(wallTex);
    wallSprite.setScale({(float)TILE_SIZE / wallTex.getSize().x, (float)TILE_SIZE / wallTex.getSize().y});
    wallSprite.setPosition({x + (TILE_SIZE * 2), y});
    tileSprites.insert({LINE_H, {"WALL", wallSprite, {x + (TILE_SIZE * 2), y}}});
    tileDisplayOrder.push_back(LINE_H);

    sf::Sprite cornerSprite(cornerTex);
    cornerSprite.setScale({(float)TILE_SIZE / cornerTex.getSize().x, (float)TILE_SIZE / cornerTex.getSize().y});
    cornerSprite.setPosition({x + (TILE_SIZE * 4), y});
    tileSprites.insert({CORNER_0, {"CORNER", cornerSprite, {x + (TILE_SIZE * 4), y}}});
    tileDisplayOrder.push_back(CORNER_0);

    sf::Sprite powerPelletSprite(pacdotTex);
    powerPelletSprite.setScale({(float)TILE_SIZE / pacdotTex.getSize().x, (float)TILE_SIZE / pacdotTex.getSize().y});
    powerPelletSprite.setPosition({x + (TILE_SIZE * 6), y});
    tileSprites.insert({POWERPELLET, {"POWER PELLET", powerPelletSprite, {x + (TILE_SIZE * 6), y}}});
    tileDisplayOrder.push_back(POWERPELLET);

    sf::Sprite ghostDoorSprite(ghostDoorTex);
    ghostDoorSprite.setScale({(float)TILE_SIZE / wallTex.getSize().x, (float)TILE_SIZE / wallTex.getSize().y});
    ghostDoorSprite.setPosition({x + (TILE_SIZE * 8), y});
    tileSprites.insert({GHOST_DOOR_H, {"GHOST DOOR", ghostDoorSprite, {x + (TILE_SIZE * 8), y}}});
    tileDisplayOrder.push_back(GHOST_DOOR_H);

    // Cherry
    sf::IntRect texRect({2 * (TILE_SIZE / 2), 3 * (TILE_SIZE / 2)}, {TILE_SIZE / 2, TILE_SIZE / 2});
    sf::Sprite fruit(gameAsset, texRect);
    fruit.setScale({2.f, 2.f});
    fruit.setPosition({x + (TILE_SIZE * 10), y});
    tileSprites.insert({FRUIT, {"FRUIT GENERATOR", fruit, {x + (TILE_SIZE * 10), y}}});
    tileDisplayOrder.push_back(FRUIT);

    // PacMan
    texRect = sf::IntRect({1 * (TILE_SIZE / 2), 1 * (TILE_SIZE / 2)}, {TILE_SIZE / 2, TILE_SIZE / 2});
    sf::Sprite pacmanSprite(gameAsset, texRect);
    pacmanSprite.setScale({2.f, 2.f});
    pacmanSprite.setPosition({x + (TILE_SIZE * 12), y});
    tileSprites.insert({PACMAN, {"PACMAN SPAWN", pacmanSprite, {x + (TILE_SIZE * 12), y}}});
    tileDisplayOrder.push_back(PACMAN);

    texRect = sf::IntRect({0, 4 * (TILE_SIZE / 2)}, {TILE_SIZE / 2, TILE_SIZE / 2});
    sf::Sprite blinkySprite(gameAsset, texRect);
    blinkySprite.setScale({2.f, 2.f});
    blinkySprite.setPosition({x + (TILE_SIZE * 14), y});
    tileSprites.insert({BLINKY, {"BLINKY", blinkySprite, {x + (TILE_SIZE * 14), y}}});
    tileDisplayOrder.push_back(BLINKY);

    texRect = sf::IntRect({0, 5 * (TILE_SIZE / 2)}, {TILE_SIZE / 2, TILE_SIZE / 2});
    sf::Sprite pinkySprite(gameAsset, texRect);
    pinkySprite.setScale({2.f, 2.f});
    pinkySprite.setPosition({x + (TILE_SIZE * 16), y});
    tileSprites.insert({PINKY, {"PINKY", pinkySprite, {x + (TILE_SIZE * 16), y}}});
    tileDisplayOrder.push_back(PINKY);

    texRect = sf::IntRect({0, 6 * (TILE_SIZE / 2)}, {TILE_SIZE / 2, TILE_SIZE / 2});
    sf::Sprite inkySprite(gameAsset, texRect);
    inkySprite.setScale({2.f, 2.f});
    inkySprite.setPosition({x + (TILE_SIZE * 18), y});
    tileSprites.insert({INKY, {"INKY", inkySprite, {x + (TILE_SIZE * 18), y}}});
    tileDisplayOrder.push_back(INKY);

    texRect = sf::IntRect({0, 7 * (TILE_SIZE / 2)}, {TILE_SIZE / 2, TILE_SIZE / 2});
    sf::Sprite clydeSprite(gameAsset, texRect);
    clydeSprite.setScale({2.f, 2.f});
    clydeSprite.setPosition({x + (TILE_SIZE * 20), y});
    tileSprites.insert({CLYDE, {"CLYDE", clydeSprite, {x + (TILE_SIZE * 20), y}}});
    tileDisplayOrder.push_back(CLYDE);

    map.resize(MAP_HEIGHT);
    for (size_t i = 0; i < MAP_HEIGHT; ++i)
    {
        map[i].resize(MAP_WIDTH);
        for (size_t j = 0; j < MAP_WIDTH; ++j)
        {
            map[i][j] = EMPTY_BLOCK;
        }
    }
}

void Create::doGraphics()
{
    Debug::drawGrid(window);
    drawCursor();
    doUI();
    drawMap();
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

    for (size_t i = 0; i < tileDisplayOrder.size(); ++i)
    {
        char tileType = tileDisplayOrder[i];
        Tile tile = tileSprites.at(tileType);
        tile.sprite.setScale(tile.sprite.getScale() * 2.f);

        window.draw(tile.sprite);
        sf::FloatRect bounds = tile.sprite.getGlobalBounds();

        if (bounds.contains({static_cast<float>(cursorPos.x) * TILE_SIZE, static_cast<float>(cursorPos.y) * TILE_SIZE}))
        {
            hoveredTileIndex = i;
        }
    }

    if (isCursorOnMap())
    {
        hoveredTileIndex = -1;
    }

    std::string selectedTileName;

    if (selectedTileIndex != -1)
    {
        sf::RectangleShape selectedTileOutline(sf::Vector2f(TILE_SIZE * 2, TILE_SIZE * 2));
        selectedTileOutline.setFillColor(sf::Color::Transparent);
        selectedTileOutline.setOutlineThickness(2.5f);
        selectedTileOutline.setOutlineColor(sf::Color::White);

        const Tile &selectedTile = tileSprites.at(tileDisplayOrder[selectedTileIndex]);
        selectedTileOutline.setPosition(selectedTile.position);
        selectedTileName = selectedTile.name;
        window.draw(selectedTileOutline);
    }

    if (hoveredTileIndex != -1)
    {
        sf::RectangleShape hoveredTileOutline(sf::Vector2f(TILE_SIZE * 2, TILE_SIZE * 2));
        hoveredTileOutline.setFillColor(sf::Color::Transparent);
        hoveredTileOutline.setOutlineThickness(2.f);
        hoveredTileOutline.setOutlineColor(sf::Color::Yellow);
        const Tile &hoveredTile = tileSprites.at(tileDisplayOrder[hoveredTileIndex]);
        hoveredTileOutline.setPosition(hoveredTile.position);
        selectedTileName = hoveredTile.name;
        window.draw(hoveredTileOutline);
    }

    arcadeText.drawString(selectedTileName, 0, MAP_HEIGHT + 3, window, TextColor::WHITE);
}

void Create::drawMap()
{
    for (size_t i = 0; i < map.size(); ++i)
    {
        for (size_t j = 0; j < map[i].size(); ++j)
        {
            char tileType = map[i][j];
            if (tileType != EMPTY_BLOCK)
            {
                if (tileSprites.find(tileType) == tileSprites.end())
                {
                    // varianti
                    switch (tileType)
                    {
                    case LINE_V:
                    {
                        Tile tile = tileSprites.at(LINE_H);
                        tile.sprite.setOrigin({tile.sprite.getLocalBounds().size.x / 2.f, tile.sprite.getLocalBounds().size.y / 2.f});
                        tile.sprite.setPosition({static_cast<float>(j) * TILE_SIZE + TILE_SIZE / 2.f, static_cast<float>(i) * TILE_SIZE + TILE_SIZE / 2.f});
                        tile.sprite.setRotation(sf::degrees(90));
                        window.draw(tile.sprite);
                        break;
                    }
                    case GHOST_DOOR_V:
                    {
                        Tile tile = tileSprites.at(GHOST_DOOR_H);
                        tile.sprite.setOrigin({tile.sprite.getLocalBounds().size.x / 2.f, tile.sprite.getLocalBounds().size.y / 2.f});
                        tile.sprite.setPosition({static_cast<float>(j) * TILE_SIZE + TILE_SIZE / 2.f, static_cast<float>(i) * TILE_SIZE + TILE_SIZE / 2.f});
                        tile.sprite.setRotation(sf::degrees(90));
                        window.draw(tile.sprite);
                        break;
                    }
                    case CORNER_90:
                    {
                        Tile tile = tileSprites.at(CORNER_0);
                        tile.sprite.setOrigin({tile.sprite.getLocalBounds().size.x / 2.f, tile.sprite.getLocalBounds().size.y / 2.f});
                        tile.sprite.setPosition({static_cast<float>(j) * TILE_SIZE + TILE_SIZE / 2.f, static_cast<float>(i) * TILE_SIZE + TILE_SIZE / 2.f});
                        tile.sprite.setRotation(sf::degrees(90));
                        window.draw(tile.sprite);
                        break;
                    }
                    case CORNER_180:
                    {
                        Tile tile = tileSprites.at(CORNER_0);
                        tile.sprite.setOrigin({tile.sprite.getLocalBounds().size.x / 2.f, tile.sprite.getLocalBounds().size.y / 2.f});
                        tile.sprite.setPosition({static_cast<float>(j) * TILE_SIZE + TILE_SIZE / 2.f, static_cast<float>(i) * TILE_SIZE + TILE_SIZE / 2.f});
                        tile.sprite.setRotation(sf::degrees(180));
                        window.draw(tile.sprite);
                        break;
                    }
                    case CORNER_270:
                    {
                        Tile tile = tileSprites.at(CORNER_0);
                        tile.sprite.setOrigin({tile.sprite.getLocalBounds().size.x / 2.f, tile.sprite.getLocalBounds().size.y / 2.f});
                        tile.sprite.setPosition({static_cast<float>(j) * TILE_SIZE + TILE_SIZE / 2.f, static_cast<float>(i) * TILE_SIZE + TILE_SIZE / 2.f});
                        tile.sprite.setRotation(sf::degrees(270));
                        window.draw(tile.sprite);
                        break;
                    }
                    default:
                        break;
                    }
                }
                else
                {
                    Tile tile = tileSprites.at(tileType);
                    tile.sprite.setPosition({static_cast<float>(j) * TILE_SIZE, static_cast<float>(i) * TILE_SIZE});
                    window.draw(tile.sprite);
                }
            }
        }
    }
}

bool Create::isCursorOnMap()
{
    return cursorPos.x >= 0 && cursorPos.x < MAP_WIDTH && cursorPos.y >= 0 && cursorPos.y < MAP_HEIGHT;
}

void Create::handle(const sf::Event::MouseButtonPressed &mouseButton)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if (isCursorOnMap() && selectedTileIndex != -1)
        {
            char tileType;
            if (lastTileType != EMPTY_BLOCK)
            {
                tileType = lastTileType;
            }
            else
            {
                tileType = tileDisplayOrder[selectedTileIndex];
            }
            map[cursorPos.y][cursorPos.x] = tileType;
        }
        else if (hoveredTileIndex != -1)
        {
            lastTileType = EMPTY_BLOCK;
            selectedTileIndex = hoveredTileIndex;
        }
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
    {
        char tileType = map[cursorPos.y][cursorPos.x];
        if (isCursorOnMap() && tileType != EMPTY_BLOCK)
        {
            map[cursorPos.y][cursorPos.x] = EMPTY_BLOCK;
        }
        else if (tileType != EMPTY_BLOCK)
        {
            selectedTileIndex = -1;
        }
    }
}

void Create::handle(const sf::Event::KeyPressed &key)
{
    if (key.scancode == sf::Keyboard::Scancode::R)
    {
        char &tileType = map[cursorPos.y][cursorPos.x];
        switch (tileType)
        {
        case LINE_H:
            tileType = LINE_V;
            lastTileType = LINE_V;
            break;
        case LINE_V:
            tileType = LINE_H;
            lastTileType = EMPTY_BLOCK;
            break;
        case GHOST_DOOR_H:
            tileType = GHOST_DOOR_V;
            lastTileType = GHOST_DOOR_V;
            break;
        case GHOST_DOOR_V:
            tileType = GHOST_DOOR_H;
            lastTileType = EMPTY_BLOCK;
            break;
        case CORNER_0:
            tileType = CORNER_90;
            lastTileType = CORNER_90;
            break;
        case CORNER_90:
            tileType = CORNER_180;
            lastTileType = CORNER_180;
            break;
        case CORNER_180:
            tileType = CORNER_270;
            lastTileType = CORNER_270;
            break;
        case CORNER_270:
            tileType = CORNER_0;
            lastTileType = EMPTY_BLOCK;
            break;
        default:
            break;
        }
    }
}