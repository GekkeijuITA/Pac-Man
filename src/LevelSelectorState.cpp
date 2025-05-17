#include "../includes/LevelSelectorState.hpp"
#include "../includes/textures.hpp"

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

    loadMaps();
};

void LevelSelectorState::loadMaps()
{
    std::string path = "../resources/maps";

    sf::View view = window.getView();

    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        std::string path = entry.path();
        sf::Texture texture = generateMapPreview(path);

        maps.push_back({prettify(path),
                        path,
                        texture,
                        sf::Vector2f(0.f, 0.f)});
    }
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
    sf::Vector2f gridSize;
    float maxSpacingX = 0.f, maxSpacingY = 0.f;
    for (const auto &map : maps)
    {
        float spacingX = map.texture.getSize().x + margin;
        float spacingY = map.texture.getSize().y + margin;
        maxSpacingX = std::max(maxSpacingX, spacingX);
        maxSpacingY = std::max(maxSpacingY, spacingY);
    }

    gridSize.x = cols * maxSpacingX;
    gridSize.y = ((maps.size() + cols - 1) / cols) * maxSpacingY;

    sf::Vector2f viewCenter = window.getView().getCenter();
    sf::Vector2f viewSize = window.getView().getSize();

    sf::Vector2f offset(margin / 2, margin / 2);
    // offset.x = viewCenter.x - (gridSize.x / 2.f);
    // offset.y = viewCenter.y - (gridSize.y / 2.f);

    for (size_t i = 0; i < maps.size(); ++i)
    {
        int row = i / cols;
        int col = i % cols;

        float previewWidth = maps[i].texture.getSize().x;
        float previewHeight = maps[i].texture.getSize().y;
        float spacingX = previewWidth + margin;
        float spacingY = previewHeight + margin;

        sf::Vector2f position(col * spacingX + offset.x, row * spacingY + offset.y);
        maps[i].position = position;
    }

    for (const auto &map : maps)
    {
        sf::Sprite mapPreview(map.texture);
        mapPreview.setPosition(map.position);
        window.draw(mapPreview);

        int textX = static_cast<int>(
            (map.position.x + (map.texture.getSize().x / 2.f) - (map.name.length() * TILE_SIZE / 2.f)) / TILE_SIZE);

        int textY = static_cast<int>(
            (map.position.y + map.texture.getSize().y + 5.f) / TILE_SIZE);
        
        // PROBLEMA QUI CON IL DISEGNO DEL TESTO
        arcadeText.drawString(map.name, 0.f, 0.f, window);
    }
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

    float TILE_SIZE_PREVIEW = TILE_SIZE / cols;
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