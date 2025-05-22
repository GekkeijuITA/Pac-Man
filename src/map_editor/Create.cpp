#include "../../includes/map_editor/Create.hpp"
#include "../../includes/core/Debug.hpp"
#include "../../includes/lib/global_values.hpp"
#include "../../includes/lib/textures.hpp"
#include "../../includes/lib/TileFactory.hpp"

#include <iostream>

Create::Create(sf::RenderWindow &window) : window(window)
{
    if (!gameAsset.loadFromFile(ASSET))
    {
        std::cerr << "Error loading fruit texture" << std::endl;
    }

    cursorPos = {0, 0};

    float x = 0.f;
    float y = MAP_HEIGHT * TILE_SIZE;

    auto tileData = TileFactory::getIstance().getTile(PACDOT);
    if (!tileData)
    {
        std::cerr << "Error loading tile data" << std::endl;
        return;
    }
    tileSprites.insert({tileData->type, {"PACDOT", tileData->sprite, {x, y}}});
    tileDisplayOrder.push_back(tileData->type);

    tileData = TileFactory::getIstance().getTile(LINE_H);
    if (!tileData)
    {
        std::cerr << "Error loading tile data" << std::endl;
        return;
    }
    tileSprites.insert({tileData->type, {"WALL", tileData->sprite, {x + (TILE_SIZE * 2), y}}});
    tileDisplayOrder.push_back(tileData->type);

    tileData = TileFactory::getIstance().getTile(CORNER_0);
    if (!tileData)
    {
        std::cerr << "Error loading tile data" << std::endl;
        return;
    }
    tileSprites.insert({tileData->type, {"CORNER", tileData->sprite, {x + (TILE_SIZE * 4), y}}});
    tileDisplayOrder.push_back(tileData->type);

    tileData = TileFactory::getIstance().getTile(POWERPELLET);
    if (!tileData)
    {
        std::cerr << "Error loading tile data" << std::endl;
        return;
    }
    tileSprites.insert({tileData->type, {"POWERPELLET", tileData->sprite, {x + (TILE_SIZE * 6), y}}});
    tileDisplayOrder.push_back(tileData->type);

    tileData = TileFactory::getIstance().getTile(GHOST_DOOR_H);
    if (!tileData)
    {
        std::cerr << "Error loading tile data" << std::endl;
        return;
    }
    tileSprites.insert({tileData->type, {"GHOST DOOR", tileData->sprite, {x + (TILE_SIZE * 8), y}}});
    tileDisplayOrder.push_back(tileData->type);

    tileData = TileFactory::getIstance().getTile(FRUIT);
    if (!tileData)
    {
        std::cerr << "Error loading tile data" << std::endl;
        return;
    }
    tileSprites.insert({tileData->type, {"FRUIT GENERATOR", tileData->sprite, {x + (TILE_SIZE * 10), y}}});
    tileDisplayOrder.push_back(tileData->type);

    tileData = TileFactory::getIstance().getTile(PACMAN);
    if (!tileData)
    {
        std::cerr << "Error loading tile data" << std::endl;
        return;
    }
    tileSprites.insert({tileData->type, {"PACMAN SPAWN", tileData->sprite, {x + (TILE_SIZE * 12), y}}});
    tileDisplayOrder.push_back(tileData->type);

    tileData = TileFactory::getIstance().getTile(BLINKY);
    if (!tileData)
    {
        std::cerr << "Error loading tile data" << std::endl;
        return;
    }
    tileSprites.insert({tileData->type, {"BLINKY", tileData->sprite, {x + (TILE_SIZE * 14), y}}});
    tileDisplayOrder.push_back(tileData->type);

    tileData = TileFactory::getIstance().getTile(PINKY);
    if (!tileData)
    {
        std::cerr << "Error loading tile data" << std::endl;
        return;
    }
    tileSprites.insert({tileData->type, {"PINKY", tileData->sprite, {x + (TILE_SIZE * 16), y}}});
    tileDisplayOrder.push_back(tileData->type);

    tileData = TileFactory::getIstance().getTile(INKY);
    if (!tileData)
    {
        std::cerr << "Error loading tile data" << std::endl;
        return;
    }
    tileSprites.insert({tileData->type, {"INKY", tileData->sprite, {x + (TILE_SIZE * 18), y}}});
    tileDisplayOrder.push_back(tileData->type);

    tileData = TileFactory::getIstance().getTile(CLYDE);
    if (!tileData)
    {
        std::cerr << "Error loading tile data" << std::endl;
        return;
    }
    tileSprites.insert({tileData->type, {"CLYDE", tileData->sprite, {x + (TILE_SIZE * 20), y}}});
    tileDisplayOrder.push_back(tileData->type);

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
        tile.sprite.setOrigin({0.f, 0.f});
        tile.sprite.setPosition(tile.position);
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
                auto tile = TileFactory::getIstance().getTile(tileType);
                if (!tile)
                {
                    std::cerr << "Error loading tile data" << std::endl;
                    break;
                }
                tile->sprite.setPosition({static_cast<float>(j) * TILE_SIZE + TILE_SIZE / 2.f, static_cast<float>(i) * TILE_SIZE + TILE_SIZE / 2.f});
                window.draw(tile->sprite);
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