#include "../../includes/map_editor/Create.hpp"
#include "../../includes/core/Debug.hpp"
#include "../../includes/lib/global_values.hpp"
#include "../../includes/lib/textures.hpp"
#include "../../includes/lib/TileFactory.hpp"

#include <iostream>
#include <fstream>

Create::Create(sf::RenderWindow &window, MapEditor &me) : window(window), mapEditor(me)
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

    options = {
        {"Save", [this]()
         {
             saveMap();
         }},
        {"Save and Exit", [this]()
         {
             saveMap();
             if (errorMessage.empty())
             {
                 reset();
                 mapEditor.currentMode = MapEditor::MENU;
             }
         }},
        {"Exit without saving", [this]()
         {
             reset();
             mapEditor.currentMode = MapEditor::MENU;
         }}};
    menu = GameMenu(window.getView(), "OPTIONS", sf::Vector2i(1, 1), TextColor::WHITE, options, sf::Vector2i(2, 4));
}

Create::Create(sf::RenderWindow &window, MapEditor &mapEditor, std::string mapName) : Create(window, mapEditor)
{
    this->mapName = mapName;
    loadMap();
}

void Create::doGraphics()
{
    drawMap();
    Debug::drawGrid(window);
    doUI();
    drawCursor();

    if (optionsMenu)
    {
        menu.draw(window);
        if (!errorMessage.empty())
        {
            arcadeText.drawString(errorMessage, 1, 12, window, TextColor::RED);
        }
        else if (!successMessage.empty())
        {
            arcadeText.drawString(successMessage, 1, 12, window, TextColor::CYAN);
        }
    }
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
    cursor.setOutlineThickness(2.f);
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
    outline.setOutlineThickness(2.f);
    outline.setOutlineColor(Debug::gridColor);
    window.draw(outline);

    for (size_t i = 0; i < tileDisplayOrder.size(); ++i)
    {
        char tileType = tileDisplayOrder[i];
        if (tileType == PACMAN && maxPacman <= 0)
            continue;
        if (tileType == BLINKY && maxBlinky <= 0)
            continue;
        if (tileType == PINKY && maxPinky <= 0)
            continue;
        if (tileType == INKY && maxInky <= 0)
            continue;
        if (tileType == CLYDE && maxClyde <= 0)
            continue;

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

    if (isCursorOnMap() || cursorPos.x >= tileDisplayOrder.size() * 2 || cursorPos.y >= MAP_HEIGHT + 2)
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

    arcadeText.drawString(selectedTileName, 0, MAP_HEIGHT + 2.3f, window, 0.5f, TextColor::WHITE);
    drawInputText();
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

void Create::drawInputText()
{
    std::string prefix = "Map Name '";
    arcadeText.drawString(prefix + mapName + "'", 0, MAP_HEIGHT + 3.5f, window, TextColor::CYAN);

    if (writingNameMap)
    {
        float textX = prefix.length();
        float textY = (MAP_HEIGHT + 3.5f);
        float scale = 1.0f;

        sf::RectangleShape cursorLine(sf::Vector2f(TILE_SIZE * scale * 0.15f, TILE_SIZE * scale * 1.2f));
        cursorLine.setFillColor(sf::Color::Cyan);
        cursorLine.setPosition({(textX + textCursorPos) * TILE_SIZE, (textY * TILE_SIZE) - TILE_SIZE * 0.1f});
        window.draw(cursorLine);
    }
}

// https://stackoverflow.com/questions/5252612/replace-space-with-an-underscore
void Create::saveMap()
{
    if (mapName.empty())
    {
        errorMessage = "Map name cannot be empty";
        successMessage.clear();
        return;
    }

    if (maxPacman > 0)
    {
        errorMessage = "You must place a pacman";
        successMessage.clear();
        return;
    }

    if (maxBlinky > 0 && maxPinky > 0 && maxInky > 0 && maxClyde > 0)
    {
        errorMessage = "You must place at least\none ghost";
        successMessage.clear();
        return;
    }

    if (pacdotPlaced == 0)
    {
        errorMessage = "You must place at least\none pacdot";
        successMessage.clear();
        return;
    }

    if (ghostDoorPlaced == 0)
    {
        errorMessage = "You must place at least\none ghost door";
        successMessage.clear();
        return;
    }

    if (maxInky == 0 && pacdotPlaced < 30)
    {
        errorMessage = "To use Inky, you need\nat least 30 pac-dots.\nYou still need " + std::to_string(30 - pacdotPlaced) + " more!";
        successMessage.clear();
        return;
    }

    if (maxClyde == 0 && pacdotPlaced < 60)
    {
        errorMessage = "To use Clyde, you need\nat least 60 pac-dots.\nYou still need " + std::to_string(60 - pacdotPlaced) + " more!";
        successMessage.clear();
        return;
    }

    std::ofstream mapFile;
    std::string safeMapName = mapName;
    std::replace(safeMapName.begin(), safeMapName.end(), ' ', '_');
    if (safeMapName.empty())
    {
        errorMessage = "Map name cannot be empty";
        successMessage.clear();
        return;
    }

    std::string filePath = folderPath + safeMapName + ".txt";
    mapFile.open(filePath, std::ios::out);
    if (!mapFile.is_open())
    {
        errorMessage = "Error opening file";
        successMessage.clear();
        return;
    }

    for (size_t i = 0; i < map.size(); i++)
    {
        mapFile << std::string(map[i].begin(), map[i].end()) << "\n";
    }

    errorMessage.clear();
    successMessage = "Map saved successfully";
    mapFile.close();
}

void Create::loadMap()
{
    std::ifstream mapFile;
    std::string filePath = folderPath + mapName + ".txt";
    mapFile.open(filePath, std::ios::in);
    if (!mapFile.is_open())
    {
        errorMessage = "Error opening file";
        std::cerr << "Error opening file: " << filePath << std::endl;
        return;
    }

    for (size_t i = 0; i < map.size(); i++)
    {
        std::string line;
        std::getline(mapFile, line);
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        if (line.length() != MAP_WIDTH)
        {
            errorMessage = "Invalid map format";
            std::cerr << "Invalid map format on line (" << i + 1 << ") " << line.length() << " != " << MAP_WIDTH << std::endl;
            return;
        }
        for (size_t j = 0; j < line.length(); j++)
        {
            switch (line[j])
            {
            case PACDOT:
                pacdotPlaced++;
                break;
            case GHOST_DOOR_H:
                ghostDoorPlaced++;
                break;
            case GHOST_DOOR_V:
                ghostDoorPlaced++;
                break;
            case PACMAN:
                maxPacman--;
                break;
            case BLINKY:
                maxBlinky--;
                break;
            case PINKY:
                maxPinky--;
                break;
            case INKY:
                maxInky--;
                break;
            case CLYDE:
                maxClyde--;
                break;
            }
        }
        std::copy(line.begin(), line.end(), map[i].begin());
    }
    mapFile.close();
}

void Create::handle(const sf::Event::MouseButtonPressed &mouseButton)
{
    if (!optionsMenu)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            if (isCursorOnMap())
            {
                if (!writingNameMap && selectedTileIndex != -1)
                {
                    drawTile();
                }
                else
                {
                    writingNameMap = false;
                }
            }
            else if (cursorPos.y >= MAP_HEIGHT + 3)
            {
                writingNameMap = true;
            }
            else if (hoveredTileIndex != -1)
            {
                writingNameMap = false;
                lastTileType = EMPTY_BLOCK;
                selectedTileIndex = hoveredTileIndex;
            }
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
        {
            if (isCursorOnMap())
            {
                deleteTile();
            }
            else
            {
                selectedTileIndex = -1;
            }
        }
    }
}

void Create::handle(const sf::Event::KeyPressed &key)
{
    if (key.scancode == sf::Keyboard::Scancode::R && !optionsMenu && isCursorOnMap())
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

    if (writingNameMap)
    {
        if (key.scancode == sf::Keyboard::Scancode::Enter || key.scancode == sf::Keyboard::Scancode::Escape)
        {
            writingNameMap = false;
        }
        else if (key.scancode == sf::Keyboard::Scancode::Left)
        {
            if (textCursorPos > 0)
                textCursorPos--;
        }
        else if (key.scancode == sf::Keyboard::Scancode::Right)
        {
            if (textCursorPos < mapName.size())
                textCursorPos++;
        }
    }
    else
    {
        if (key.scancode == sf::Keyboard::Scancode::Escape)
        {
            optionsMenu = !optionsMenu;
            menu.resetCursor();
            errorMessage.clear();
            successMessage.clear();
        }
    }
}

void Create::handle(const sf::Event::TextEntered &textEntered)
{
    if (writingNameMap)
    {
        if (textEntered.unicode == '\b')
        {
            if (textCursorPos == 0)
                return;
            textCursorPos--;
            if (textCursorPos < mapName.size())
                mapName.erase(textCursorPos, 1);
        }
        else if (mapName.size() < 16)
        {
            if (std::isalnum(textEntered.unicode) || textEntered.unicode == '!' || textEntered.unicode == '-' || textEntered.unicode == ' ')
            {
                mapName.insert(textCursorPos, 1, static_cast<char>(textEntered.unicode));
                textCursorPos++;
            }
        }
    }
}

// https://en.sfml-dev.org/forums/index.php?topic=13412.0
void Create::handle(const sf::Event::MouseMoved &mouseMoved)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    setCursorPos(static_cast<int>(worldPos.x / TILE_SIZE), static_cast<int>(worldPos.y / TILE_SIZE));

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if (isCursorOnMap() && selectedTileIndex != -1 && !writingNameMap)
        {
            drawTile();
        }
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
    {
        if (isCursorOnMap())
        {
            deleteTile();
        }
    }
}

void Create::drawTile()
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

    if (tileType == PACMAN && maxPacman <= 0)
        return;
    if (tileType == BLINKY && maxBlinky <= 0)
        return;
    if (tileType == PINKY && maxPinky <= 0)
        return;
    if (tileType == INKY && maxInky <= 0)
        return;
    if (tileType == CLYDE && maxClyde <= 0)
        return;

    if (map[cursorPos.y][cursorPos.x] != EMPTY_BLOCK)
    {
        deleteTile();
    }

    map[cursorPos.y][cursorPos.x] = tileType;

    if (tileType == PACMAN)
    {
        maxPacman--;
        selectedTileIndex = -1;
        lastTileType = EMPTY_BLOCK;
    }
    else if (tileType == BLINKY)
    {
        maxBlinky--;
        selectedTileIndex = -1;
        lastTileType = EMPTY_BLOCK;
    }
    else if (tileType == PINKY)
    {
        maxPinky--;
        selectedTileIndex = -1;
        lastTileType = EMPTY_BLOCK;
    }
    else if (tileType == INKY)
    {
        maxInky--;
        selectedTileIndex = -1;
        lastTileType = EMPTY_BLOCK;
    }
    else if (tileType == CLYDE)
    {
        maxClyde--;
        selectedTileIndex = -1;
        lastTileType = EMPTY_BLOCK;
    }
    else if (tileType == PACDOT)
    {
        pacdotPlaced++;
    }
    else if (tileType == GHOST_DOOR_H || tileType == GHOST_DOOR_V)
    {
        ghostDoorPlaced++;
    }
}

void Create::deleteTile()
{
    char tileType = map[cursorPos.y][cursorPos.x];
    map[cursorPos.y][cursorPos.x] = EMPTY_BLOCK;
    if (tileType == PACMAN)
    {
        maxPacman++;
    }
    else if (tileType == BLINKY)
    {
        maxBlinky++;
    }
    else if (tileType == PINKY)
    {
        maxPinky++;
    }
    else if (tileType == INKY)
    {
        maxInky++;
    }
    else if (tileType == CLYDE)
    {
        maxClyde++;
    }
    else if (tileType == PACDOT)
    {
        pacdotPlaced--;
    }
    else if (tileType == GHOST_DOOR_H || tileType == GHOST_DOOR_V)
    {
        ghostDoorPlaced--;
    }
}

void Create::reset()
{
    map.clear();

    optionsMenu = false;

    successMessage.clear();
    errorMessage.clear();
    mapName.clear();

    textCursorPos = 0;
    pacdotPlaced = 0;
    ghostDoorPlaced = 0;
    hoveredTileIndex = -1;
    selectedTileIndex = -1;
    maxPacman = 1;
    maxBlinky = 1;
    maxInky = 1;
    maxPinky = 1;
    maxClyde = 1;

    menu.resetCursor();
}