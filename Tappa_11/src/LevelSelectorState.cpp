#include "../includes/LevelSelectorState.hpp"
#include "../includes/textures.hpp"
#include "../includes/Debug.hpp"

LevelSelectorState::LevelSelectorState(sf::RenderWindow &window) : window(window)
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

    TILE_SIZE_PREVIEW = TILE_SIZE / maxCols;

    loadMaps();
};

void LevelSelectorState::loadMaps()
{
    std::string path = "../Tappa_11/resources/maps";

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

// https://stackoverflow.com/questions/6417817/easy-way-to-remove-extension-from-a-filename
// https://stackoverflow.com/questions/28963810/replace-or-remove-characters-that-are-not-in-regex-with-c
std::string LevelSelectorState::prettify(std::string name)
{
    size_t lastIndex;
    std::string rawName;
    lastIndex = name.find_last_of("/");
    rawName = name.substr(lastIndex + 1, std::string::npos);
    lastIndex = rawName.find_last_of(".");
    rawName = rawName.substr(0, lastIndex);

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
    arcadeText.drawString(title, 0, 0.2f, window, 1.f);

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

            arcadeText.drawString(map.name, nameX, nameY, window, scaleFactor);
        }
    }

    float windowHeight = view.getSize().y;
    int textY = static_cast<int>((windowHeight - TILE_SIZE * 2) / TILE_SIZE);

    std::string pageString = "Page " + std::to_string(page + 1) + " of " + std::to_string(totalPages);
    float centerX = (view.getSize().x - pageString.length() * (TILE_SIZE / 2.f)) / (TILE_SIZE * 2.f);
    arcadeText.drawString(pageString, centerX, textY - 1.5f, window);

    arcadeText.drawString("Press Enter to play", 0, textY, window, 0.7f);
    arcadeText.drawString("Press ESC to go back", 0, textY + 1, window, 0.7f);

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
            switch (map[r][c])
            {
            case PACDOT:
            {
                sf::CircleShape pacdot(TILE_SIZE_PREVIEW / 10.f);
                pacdot.setPosition({x + (TILE_SIZE_PREVIEW / 2) - 4, y + (TILE_SIZE_PREVIEW / 2) - 4});
                pacdot.setFillColor(sf::Color(255, 185, 176));
                renderTexture.draw(pacdot);
                break;
            }
            case LINE_V:
            {
                sf::Sprite wall(straightLine);
                wall.setScale({(float)TILE_SIZE_PREVIEW / straightLine.getSize().x, (float)TILE_SIZE_PREVIEW / straightLine.getSize().y});
                wall.setPosition({x, y});
                renderTexture.draw(wall);
                break;
            }
            case LINE_H:
            {
                sf::Sprite wall(straightLine);
                wall.setScale({(float)TILE_SIZE_PREVIEW / straightLine.getSize().x, (float)TILE_SIZE_PREVIEW / straightLine.getSize().y});
                wall.setOrigin({straightLine.getSize().x / 2.f,
                                straightLine.getSize().y / 2.f});
                wall.setPosition({x + TILE_SIZE_PREVIEW / 2.f, y + TILE_SIZE_PREVIEW / 2.f});
                wall.setRotation(sf::degrees(-90));
                renderTexture.draw(wall);
                break;
            }
            case CORNER_0:
            {
                sf::Sprite wall(cornerTile);
                wall.setScale({(float)TILE_SIZE_PREVIEW / cornerTile.getSize().x, (float)TILE_SIZE_PREVIEW / cornerTile.getSize().y});
                wall.setPosition({x, y});
                renderTexture.draw(wall);
                break;
            }
            case CORNER_90:
            {
                sf::Sprite wall(cornerTile);
                wall.setScale({(float)TILE_SIZE_PREVIEW / cornerTile.getSize().x, (float)TILE_SIZE_PREVIEW / cornerTile.getSize().y});
                wall.setOrigin({cornerTile.getSize().x / 2.f,
                                cornerTile.getSize().y / 2.f});

                wall.setPosition({x + TILE_SIZE_PREVIEW / 2.f, y + TILE_SIZE_PREVIEW / 2.f});
                wall.setRotation(sf::degrees(-90));
                renderTexture.draw(wall);
                break;
            }
            case CORNER_180:
            {
                sf::Sprite wall(cornerTile);
                wall.setScale({(float)TILE_SIZE_PREVIEW / cornerTile.getSize().x, (float)TILE_SIZE_PREVIEW / cornerTile.getSize().y});
                wall.setOrigin({cornerTile.getSize().x / 2.f,
                                cornerTile.getSize().y / 2.f});

                wall.setPosition({x + TILE_SIZE_PREVIEW / 2.f, y + TILE_SIZE_PREVIEW / 2.f});
                wall.setRotation(sf::degrees(-180));
                renderTexture.draw(wall);
                break;
            }
            case CORNER_270:
            {
                sf::Sprite wall(cornerTile);
                wall.setScale({(float)TILE_SIZE_PREVIEW / cornerTile.getSize().x, (float)TILE_SIZE_PREVIEW / cornerTile.getSize().y});
                wall.setOrigin({cornerTile.getSize().x / 2.f,
                                cornerTile.getSize().y / 2.f});

                wall.setPosition({x + TILE_SIZE_PREVIEW / 2.f, y + TILE_SIZE_PREVIEW / 2.f});
                wall.setRotation(sf::degrees(-270));
                renderTexture.draw(wall);
                break;
            }
            case POWERPELLET:
            {
                float scale = 3.f;
                sf::CircleShape powerpellet(TILE_SIZE_PREVIEW / scale);
                powerpellet.setOrigin({TILE_SIZE_PREVIEW / scale, TILE_SIZE_PREVIEW / scale});
                powerpellet.setPosition({x + (TILE_SIZE_PREVIEW / 2), y + (TILE_SIZE_PREVIEW / 2)});
                powerpellet.setFillColor(sf::Color(255, 185, 176));
                renderTexture.draw(powerpellet);
                break;
            }
            case GHOST_DOOR:
            {
                sf::RectangleShape ghostDoor({TILE_SIZE_PREVIEW, TILE_SIZE_PREVIEW / 4});
                ghostDoor.setPosition({x, y + TILE_SIZE_PREVIEW / 1.8f});
                ghostDoor.setFillColor(sf::Color(255, 203, 255));
                renderTexture.draw(ghostDoor);
                break;
            }
            case EMPTY_BLOCK:
            {
                sf::RectangleShape emptyBlock({TILE_SIZE_PREVIEW, TILE_SIZE_PREVIEW});
                emptyBlock.setPosition({x, y});
                emptyBlock.setFillColor(sf::Color::Black);
                renderTexture.draw(emptyBlock);
                break;
            }
            default:
                break;
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