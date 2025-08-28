#include "../includes/LevelSelectorState.hpp"
#include "../includes/lib/textures.hpp"
#include "../includes/core/Debug.hpp"
#include "../includes/lib/TileFactory.hpp"
#include "../includes/lib/SoundManager.hpp"

LevelSelectorState::LevelSelectorState(sf::RenderWindow &window, StateManager &sm, MapEditor &me) : window(window), stateManager(sm), mapEditor(me)
{
    r = std::regex("[^a-zA-Z0-9!,\",-,/]");

    if (!straightLine.loadFromFile(STRAIGHT_LINE_H))
    {
        std::cerr << "Errore nel caricamento di STRAIGHT_LINE_H\n";
    }

    if (!cornerTile.loadFromFile(ANGLE_0))
    {
        std::cerr << "Errore nel caricamento di ANGLE_0\n";
    }

    if (!pacDot.loadFromFile(PACDOT_TEX))
    {
        std::cerr << "Errore nel caricamento di PACDOT\n";
    }

    TILE_SIZE_PREVIEW = TILE_SIZE / maxCols;
};

void LevelSelectorState::loadMaps()
{
    std::string path = "../../Tappa_14/resources/maps";

    sf::View view = window.getView();
    maps.clear();
    maps.push_back({});

    int r = 0;
    int c = 0;

    float spacingX = (TILE_SIZE_PREVIEW * MAP_WIDTH) + margin;
    float spacingY = (TILE_SIZE_PREVIEW * MAP_HEIGHT) + margin;
    sf::Vector2f offset(margin / 2, margin / 2);

    int mapsLoaded = 0;
    int currentPage = 0;

    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        std::string path = entry.path();

        if (stateManager.currentMode == StateManager::MAP_EDITOR && getMapName(path) == "classic")
        {
            continue;
        }

        sf::Texture texture = generateMapPreview(path);

        if (c == maxCols)
        {
            c = 0;
            r++;
            maps.push_back({});
        }

        if (r >= maxRows)
        {
            currentPage++;
            c = 0;
        }

        sf::Vector2f position(c * spacingX + offset.x,
                              (r * spacingY + offset.y + TILE_SIZE) - (currentPage * maxRows * spacingY));

        maps[r].push_back({prettify(path),
                           path,
                           texture,
                           position});

        mapsLoaded++;
        c++;
    }

    double mapsPerPage = maxRows * maxCols;

    page = 0;
    totalPages = std::ceil(mapsLoaded / mapsPerPage);
}

std::string LevelSelectorState::getMapName(std::string path)
{
    size_t lastIndex;
    std::string rawName;
    lastIndex = path.find_last_of("/");
    rawName = path.substr(lastIndex + 1, std::string::npos);
    lastIndex = rawName.find_last_of(".");
    rawName = rawName.substr(0, lastIndex);
    return rawName;
}

// https://stackoverflow.com/questions/6417817/easy-way-to-remove-extension-from-a-filename
// https://stackoverflow.com/questions/28963810/replace-or-remove-characters-that-are-not-in-regex-with-c
std::string LevelSelectorState::prettify(std::string name)
{
    std::string rawName = getMapName(name);
    std::stringstream result;
    std::regex_replace(
        std::ostream_iterator<char>(result),
        rawName.begin(),
        rawName.end(),
        r,
        " ");

    return result.str();
}

void LevelSelectorState::draw()
{
    sf::View view = window.getView();

    std::string title = "Map Selector";
    arcadeText.drawString(title, 0, 0.2f, window, 1.f, TextColor::WHITE);

    size_t startRow = page * maxRows;
    size_t endRow = std::min(startRow + maxRows, maps.size());

    for (size_t r = startRow; r < endRow; ++r)
    {
        for (size_t c = 0; c < maps[r].size(); ++c)
        {
            MapPreview &map = maps[r][c];

            sf::Sprite mapPreview(map.texture);
            mapPreview.setPosition(map.position);
            window.draw(mapPreview);

            float mapCenterX = map.position.x + ((TILE_SIZE_PREVIEW * MAP_WIDTH) / 2.f);
            float nameX = (mapCenterX - (map.name.length() * TILE_SIZE * 0.25f)) / TILE_SIZE;
            float nameY = (map.position.y + (MAP_HEIGHT * TILE_SIZE_PREVIEW)) / TILE_SIZE;

            float scaleFactor = std::min(
                (TILE_SIZE_PREVIEW * MAP_WIDTH) / (map.name.length() * TILE_SIZE),
                .5f);

            arcadeText.drawString(map.name, nameX, nameY, window, scaleFactor, TextColor::WHITE);
        }
    }

    float windowHeight = view.getSize().y;
    int textY = static_cast<int>((windowHeight - TILE_SIZE * 2) / TILE_SIZE);

    std::string pageString = "Page " + std::to_string(page + 1) + " of " + std::to_string(totalPages);
    float centerX = (view.getSize().x - pageString.length() * (TILE_SIZE / 2.f)) / (TILE_SIZE * 2.f);
    arcadeText.drawString(pageString, centerX, textY - 1.5f, window, TextColor::WHITE);

    std::string text = "play";
    if (stateManager.currentMode == StateManager::MAP_EDITOR)
    {
        text = "edit";
    }
    arcadeText.drawString("Press Enter to " + text, 0, textY, window, 0.7f, TextColor::WHITE);
    arcadeText.drawString("Press ESC to go back", 0, textY + 1, window, 0.7f, TextColor::WHITE);

    drawCursor();
}

sf::Texture LevelSelectorState::generateMapPreview(const std::string path)
{
    std::vector<std::vector<char>> map;
    std::fstream mapFile;
    mapFile.open(path, std::ios::in);
    if (!mapFile.is_open())
    {
        std::cerr << "Errore nell'apertura del file contenente la mappa" << std::endl;
        return sf::Texture();
    }

    std::string line;

    while (std::getline(mapFile, line))
    {
        std::vector<char> row(line.begin(), line.end());
        map.push_back(row);
    }

    int mapHeight = map.size();
    int mapWidth = (mapHeight > 0) ? map[0].size() : 0;

    float TILE_SIZE_PREVIEW = TILE_SIZE / maxCols;
    sf::RenderTexture renderTexture({static_cast<unsigned int>(mapWidth * TILE_SIZE_PREVIEW),
                                     static_cast<unsigned int>(mapHeight * TILE_SIZE_PREVIEW)});
    renderTexture.clear(sf::Color::Black);

    for (int r = 0; r < mapHeight; r++)
    {
        float y = r * TILE_SIZE_PREVIEW;
        for (int c = 0; c < mapWidth; c++)
        {
            float x = c * TILE_SIZE_PREVIEW;
            char tileType = map[r][c];

            if (tileType != EMPTY_BLOCK && tileType)
            {
                auto tile = TileFactory::getIstance().getTile(tileType);
                if (!tile)
                {
                    std::cerr << "Error loading tile data" << std::endl;
                    break;
                }
                tile->sprite.setPosition({static_cast<float>(c) * TILE_SIZE_PREVIEW + TILE_SIZE_PREVIEW / 2.f, static_cast<float>(r) * TILE_SIZE_PREVIEW + TILE_SIZE_PREVIEW / 2.f});
                tile->sprite.setScale({(float)TILE_SIZE_PREVIEW / tile->sprite.getTexture().getSize().x, (float)TILE_SIZE_PREVIEW / tile->sprite.getTexture().getSize().y});
                renderTexture.draw(tile->sprite);
            }
        }
    }
    renderTexture.display();

    sf::Texture texture = renderTexture.getTexture();
    return texture;
}

void LevelSelectorState::drawCursor()
{
    sf::Vector2f offset(margin / 2, margin / 2);
    sf::RectangleShape cursor(sf::Vector2f(TILE_SIZE_PREVIEW * MAP_WIDTH, (TILE_SIZE_PREVIEW * MAP_HEIGHT) + (TILE_SIZE / 2)));
    cursor.setFillColor(sf::Color::Transparent);
    cursor.setOutlineColor(sf::Color::Red);
    cursor.setOutlineThickness(2.f);

    sf::Vector2f pos(
        cursorPosition.x * (TILE_SIZE_PREVIEW * MAP_WIDTH + margin) + offset.x,
        cursorPosition.y * (TILE_SIZE_PREVIEW * MAP_HEIGHT + margin) + offset.y + TILE_SIZE);

    cursor.setPosition(pos);
    window.draw(cursor);
}

void LevelSelectorState::moveCursorLeft()
{
    if (cursorPosition.x > 0)
    {
        cursorPosition.x--;
    }
    else if (cursorPosition.y != 0)
    {
        cursorPosition.x = maps[cursorPosition.y].size() - 1;
        moveCursorUp();
    }
    else
    {
        previousPage();
    }
}

void LevelSelectorState::moveCursorRight()
{
    int currentRow = page * maxRows + cursorPosition.y;
    if (currentRow >= static_cast<int>(maps.size()))
    {
        return;
    }

    if (cursorPosition.x < static_cast<int>(maps[currentRow].size()) - 1)
    {
        cursorPosition.x++;
    }
    else if (currentRow != maxRows - 1 && currentRow < static_cast<int>(maps.size()) - 1)
    {
        cursorPosition.x = 0;
        moveCursorDown();
    }
    else
    {
        nextPage();
    }
}

void LevelSelectorState::moveCursorUp()
{
    if (cursorPosition.y > 0)
    {
        cursorPosition.y--;
    }
    else if (cursorPosition.y == 0)
    {
        previousPage();
    }
}

void LevelSelectorState::moveCursorDown()
{
    int rows = std::min(
        maxRows,
        static_cast<int>(maps.size() - page * maxRows));

    if (cursorPosition.y < rows - 1)
    {
        cursorPosition.y++;
        if (cursorPosition.x >= static_cast<int>(maps[cursorPosition.y].size()))
        {
            cursorPosition.x = static_cast<int>(maps[cursorPosition.y].size() - 1);
        }
    }
    else if (cursorPosition.y == rows - 1)
    {
        nextPage();
    }
}

void LevelSelectorState::nextPage()
{
    if (page < totalPages - 1)
    {
        page++;
        cursorPosition.x = 0;
        cursorPosition.y = 0;
    }
}

void LevelSelectorState::previousPage()
{
    if (page > 0)
    {
        page--;
        cursorPosition.x = maps[cursorPosition.y].size() - 1;
        cursorPosition.y = maps[cursorPosition.y].size() - 1;
    }
}

void LevelSelectorState::handle(const sf::Event::KeyPressed &key)
{
    if (key.scancode == sf::Keyboard::Scancode::Up)
    {
        moveCursorUp();
    }
    else if (key.scancode == sf::Keyboard::Scancode::Down)
    {
        moveCursorDown();
    }
    else if (key.scancode == sf::Keyboard::Scancode::Left)
    {
        moveCursorLeft();
    }
    else if (key.scancode == sf::Keyboard::Scancode::Right)
    {
        moveCursorRight();
    }
    else if (key.scancode == sf::Keyboard::Scancode::Escape)
    {
        if (stateManager.currentMode == StateManager::MAP_EDITOR)
        {
            mapEditor.currentMode = MapEditor::MENU;
        }
        else
        {
            stateManager.currentMode = StateManager::MAIN_MENU;
            stateManager.mainMenuState->getHighscore();
        }
    }
    else if (key.scancode == sf::Keyboard::Scancode::Enter)
    {
        if (maps.empty())
        {
            return;
        }
        SoundManager::getInstance().playSound("credit");
        while (SoundManager::getInstance().isSoundPlaying("credit"))
            ;

        std::string mapPath = maps[cursorPosition.y][cursorPosition.x].path;
        if (stateManager.currentMode == StateManager::LEVEL_SELECTOR)
        {
            stateManager.initGame(mapPath);
        }
        else if (stateManager.currentMode == StateManager::MAP_EDITOR)
        {
            mapEditor.initEditor(getMapName(mapPath));
        }
    }
}