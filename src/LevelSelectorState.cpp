#include "../includes/LevelSelectorState.hpp"
#include "../includes/textures.hpp"

LevelSelectorState::LevelSelectorState(sf::RenderWindow &window) : window(window)
{
    r = std::regex("[^a-zA-Z0-9!,\",-,/]");
    loadMaps();
    straightLine.loadFromFile(STRAIGHT_LINE_H);
    cornerTile.loadFromFile(ANGLE_0);
};

void LevelSelectorState::loadMaps()
{
    std::string path = "../resources/maps";

    sf::View view = window.getView();

    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        std::string path = entry.path();

        maps.push_back({prettify(path),
                        path,
                        generateMapPreview(path),
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
    gridSize.x = cols * spacing;
    gridSize.y = (maps.size() / cols + 1) * spacing;

    sf::Vector2f viewCenter = window.getView().getCenter();
    sf::Vector2f viewSize = window.getView().getSize();

    sf::Vector2f offset;
    offset.x = viewCenter.x - (gridSize.x / 2.f);
    offset.y = viewCenter.y - (gridSize.y / 2.f);

    for (size_t i = 0; i < maps.size(); ++i)
    {
        int row = i / cols;
        int col = i % cols;

        sf::Vector2f position;
        position.x = col * spacing + offset.x;
        position.y = row * spacing + offset.y;

        maps[i].position = position;
    }

    for (const auto &map : maps)
    {
        window.draw(map.sprite);
    }
}

sf::Sprite LevelSelectorState::generateMapPreview(const std::string path)
{
    // Carica la mappa da file
    std::vector<std::vector<char>> map;
    std::fstream mapFile;
    mapFile.open(path, std::ios::in);
    if (!mapFile.is_open())
    {
        std::cerr << "Errore nell'apertura del file contenente la mappa" << std::endl;
        return sf::Sprite();
    }

    std::string line;

    while (std::getline(mapFile, line))
    {
        std::vector<char> row(line.begin(), line.end());
        map.push_back(row);
    }

    const int TILE_SIZE_PREVIEW = 4;
    int mapHeight = map.size();
    int mapWidth = (mapHeight > 0) ? map[0].size() : 0;

    sf::RenderTexture renderTexture({mapWidth * TILE_SIZE_PREVIEW, mapHeight * TILE_SIZE_PREVIEW});

    renderTexture.clear(sf::Color::Black);

    sf::RectangleShape tileShape(sf::Vector2f(TILE_SIZE_PREVIEW, TILE_SIZE_PREVIEW));
    float scale = static_cast<float>(TILE_SIZE_PREVIEW) / TILE_SIZE;
    sf::Vector2f scaleFactor(scale, scale);

    for (int i = 0; i < mapHeight; ++i)
    {
        for (int j = 0; j < mapWidth; ++j)
        {
            char tile = map[i][j];
            float x = j * TILE_SIZE_PREVIEW;
            float y = i * TILE_SIZE_PREVIEW;

            switch (tile)
            {
            case PACDOT:
            {
                sf::CircleShape pacdot(TILE_SIZE / 10.f);
                pacdot.setPosition({x + (TILE_SIZE / 2) - 4, y + (TILE_SIZE / 2) - 4});
                pacdot.setFillColor(sf::Color(255, 185, 176));
                window.draw(pacdot);
                break;
            }
            case LINE_V:
            {
                sf::Sprite wall(straightLine);
                wall.setScale(scaleFactor);
                wall.setPosition({x, y});
                window.draw(wall);
                break;
            }
            case LINE_H:
            {
                sf::Sprite wall(straightLine);
                wall.setScale(scaleFactor);
                wall.setOrigin({straightLine.getSize().x / 2.f,
                                straightLine.getSize().y / 2.f});
                wall.setPosition({x + TILE_SIZE / 2.f, y + TILE_SIZE / 2.f});
                wall.setRotation(sf::degrees(-90));
                window.draw(wall);
                break;
            }
            case CORNER_0:
            {
                sf::Sprite wall(cornerTile);
                wall.setScale(scaleFactor);
                wall.setPosition({x, y});
                window.draw(wall);
                break;
            }
            case CORNER_90:
            {
                sf::Sprite wall(cornerTile);
                wall.setScale(scaleFactor);
                wall.setOrigin({cornerTile.getSize().x / 2.f,
                                cornerTile.getSize().y / 2.f});

                wall.setPosition({x + TILE_SIZE / 2.f, y + TILE_SIZE / 2.f});
                wall.setRotation(sf::degrees(-90));
                window.draw(wall);
                break;
            }
            case CORNER_180:
            {
                sf::Sprite wall(cornerTile);
                wall.setScale(scaleFactor);
                wall.setOrigin({cornerTile.getSize().x / 2.f,
                                cornerTile.getSize().y / 2.f});

                wall.setPosition({x + TILE_SIZE / 2.f, y + TILE_SIZE / 2.f});
                wall.setRotation(sf::degrees(-180));
                window.draw(wall);
                break;
            }
            case CORNER_270:
            {
                sf::Sprite wall(cornerTile);
                wall.setScale(scaleFactor);
                wall.setOrigin({cornerTile.getSize().x / 2.f,
                                cornerTile.getSize().y / 2.f});

                wall.setPosition({x + TILE_SIZE / 2.f, y + TILE_SIZE / 2.f});
                wall.setRotation(sf::degrees(-270));
                window.draw(wall);
                break;
            }
            case POWERPELLET:
            {
                float scale = 3.f;
                sf::CircleShape powerpellet(TILE_SIZE / scale);
                powerpellet.setOrigin({TILE_SIZE / scale, TILE_SIZE / scale});
                powerpellet.setPosition({x + (TILE_SIZE / 2), y + (TILE_SIZE / 2)});
                powerpellet.setFillColor(sf::Color(255, 185, 176));
                window.draw(powerpellet);
                break;
            }
            case GHOST_DOOR:
            {
                sf::RectangleShape ghostDoor({TILE_SIZE, TILE_SIZE / 4});
                ghostDoor.setPosition({x, y + TILE_SIZE / 1.8f});
                ghostDoor.setFillColor(sf::Color(255, 203, 255));
                window.draw(ghostDoor);
                break;
            }
            case EMPTY_BLOCK:
            {
                sf::RectangleShape emptyBlock({TILE_SIZE, TILE_SIZE});
                emptyBlock.setPosition({x, y});
                emptyBlock.setFillColor(sf::Color::Black);
                window.draw(emptyBlock);
                break;
            }
            default:
                break;
            }

            tileShape.setPosition({j * TILE_SIZE_PREVIEW, i * TILE_SIZE_PREVIEW});
            renderTexture.draw(tileShape);
        }
    }

    renderTexture.display();

    sf::Sprite previewSprite(renderTexture.getTexture());
    previewSprite.setScale({2.f, 2.f});

    return previewSprite;
}