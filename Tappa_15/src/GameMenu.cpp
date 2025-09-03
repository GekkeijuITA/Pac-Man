#include "../includes/GameMenu.hpp"
#include "../includes/lib/SoundManager.hpp"

#include <iostream>

GameMenu::GameMenu(std::string title, sf::Vector2i titlePos, TextColor textColor, std::vector<MenuOption> options, sf::Vector2i startOptionsPos, sf::RenderWindow &window) : title(title),
                                                                                                                                                                             options(options),
                                                                                                                                                                             titlePos(titlePos),
                                                                                                                                                                             startOptionsPos(startOptionsPos),
                                                                                                                                                                             textColor(textColor),
                                                                                                                                                                             window(window)
{
    view = window.getView();

    if (!tex.loadFromFile(TEXT))
    {
        std::cerr << "Error loading texture" << std::endl;
        exit(1);
    }

    ArcadeText arcadeText;
    setCursorPosition(startOptionsPos.x - 1, startOptionsPos.y);

    optionsBoxHeight = (options.size() * TILE_SIZE) * 2;

    int s = 0;
    for (auto opt : options)
    {
        size_t opt_size = opt.name.size();
        if (opt_size > s)
        {
            s = opt_size;
        }
    }

    optionsBoxWidth = s * TILE_SIZE;

    optionsBox = sf::FloatRect({static_cast<float>(startOptionsPos.x) * TILE_SIZE, static_cast<float>(startOptionsPos.y) * TILE_SIZE}, {optionsBoxWidth, optionsBoxHeight});
}

GameMenu::GameMenu(std::string title, sf::Vector2i titlePos, TextColor textColor, float scaleFactor, std::vector<MenuOption> options, sf::Vector2i startOptionsPos, sf::RenderWindow &window) : title(title),
                                                                                                                                                                                                options(options),
                                                                                                                                                                                                titlePos(titlePos),
                                                                                                                                                                                                startOptionsPos(startOptionsPos),
                                                                                                                                                                                                textColor(textColor),
                                                                                                                                                                                                scaleFactor(scaleFactor),
                                                                                                                                                                                                window(window)
{

    view = window.getView();

    if (!tex.loadFromFile(TEXT))
    {
        std::cerr << "Error loading texture" << std::endl;
        exit(1);
    }

    ArcadeText arcadeText;
    setCursorPosition(startOptionsPos.x - 1, startOptionsPos.y);

    optionsBoxHeight = (options.size() * TILE_SIZE) * 2;

    int s = 0;
    for (auto opt : options)
    {
        size_t opt_size = opt.name.size();
        if (opt_size > s)
        {
            s = opt_size;
        }
    }

    optionsBoxWidth = s * TILE_SIZE;

    optionsBox = sf::FloatRect({static_cast<float>(startOptionsPos.x) * TILE_SIZE, static_cast<float>(startOptionsPos.y) * TILE_SIZE}, {optionsBoxWidth, optionsBoxHeight});
}

void GameMenu::draw()
{
    sf::RectangleShape background(view.getSize());
    background.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(background);

    arcadeText.drawString(title, titlePos.x, titlePos.y, window, scaleFactor, textColor);
    for (auto &option : options)
    {
        arcadeText.drawString(option.name, startOptionsPos.x, startOptionsPos.y + (2 * (&option - &options[0])), window, TextColor::WHITE);
    }

    sf::CircleShape triangle(TILE_SIZE / 2.f, 3);
    triangle.setFillColor(sf::Color::Red);
    triangle.setPosition({TILE_SIZE * (cursorPosition.x + .5f), TILE_SIZE * ((cursorPosition.y + .5f))});
    triangle.setOrigin({TILE_SIZE / 2.f, TILE_SIZE / 2.f});
    triangle.setRotation(sf::degrees(90));
    window.draw(triangle);
}

void GameMenu::setCursorPosition(int x, int y)
{
    cursorPosition = {x, y};
}

sf::Vector2i GameMenu::getCursorPosition()
{
    return cursorPosition;
}

int GameMenu::getOptionsSize()
{
    return options.size();
}

// Muove il cursore verso l'alto
void GameMenu::moveCursorUp()
{
    if (cursorIndex > 0)
    {
        cursorIndex--;
    }
    else
    {
        cursorIndex = getOptionsSize() - 1;
    }

    setCursorPosition(cursorPosition.x, startOptionsPos.y + 2 * cursorIndex);
}

// Muove il cursore verso il basso
void GameMenu::moveCursorDown()
{
    if (cursorIndex < getOptionsSize() - 1)
    {
        cursorIndex++;
    }
    else
    {
        cursorIndex = 0;
    }

    setCursorPosition(cursorPosition.x, startOptionsPos.y + 2 * cursorIndex);
}

// Esegue l'azione associata all'opzione selezionata
void GameMenu::executeOption()
{
    options[cursorIndex].action();
}

void GameMenu::resetCursor()
{
    cursorIndex = 0;
    setCursorPosition(cursorPosition.x, startOptionsPos.y);
}

void GameMenu::setOptions(const std::vector<MenuOption> &options)
{
    this->options = options;

    optionsBoxHeight = (options.size() * TILE_SIZE) * 2;

    int s = 0;
    for (auto opt : options)
    {
        size_t opt_size = opt.name.size();
        if (opt_size > s)
        {
            s = opt_size;
        }
    }

    optionsBoxWidth = s * TILE_SIZE;
    optionsBox = sf::FloatRect({static_cast<float>(startOptionsPos.x) * TILE_SIZE, static_cast<float>(startOptionsPos.y) * TILE_SIZE}, {optionsBoxWidth, optionsBoxHeight});


    std::cout << "Menu options updated" << std::endl;
    std::cout << "New options box size: " << optionsBoxWidth << "x" << optionsBoxHeight << std::endl;
}

void GameMenu::handle(const sf::Event::KeyPressed &key)
{
    switch (key.scancode)
    {
    case sf::Keyboard::Scancode::Up:
        moveCursorUp();
        break;
    case sf::Keyboard::Scancode::Down:
        moveCursorDown();
        break;
    case sf::Keyboard::Scancode::Enter:
        SoundManager::getInstance().playSound("credit");
        executeOption();
        break;
    default:
        break;
    }
}

void GameMenu::handle(const sf::Event::MouseMoved &mouse)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

    if (optionsBox.contains(worldPos))
    {
        cursorIndex = (worldPos.y - optionsBox.position.y) / (2 * TILE_SIZE);
        setCursorPosition(cursorPosition.x, startOptionsPos.y + 2 * cursorIndex);
    }
}

void GameMenu::handle(const sf::Event::MouseButtonPressed &mouse)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

    sf::FloatRect optionsBox({static_cast<float>(startOptionsPos.x) * TILE_SIZE, static_cast<float>(startOptionsPos.y) * TILE_SIZE}, {optionsBoxWidth, optionsBoxHeight});

    if (optionsBox.contains(worldPos))
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            SoundManager::getInstance().playSound("credit");
            executeOption();
        }
    }
}
