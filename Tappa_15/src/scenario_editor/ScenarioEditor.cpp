#include "../../includes/scenario_editor/ScenarioEditor.hpp"

ScenarioEditor::ScenarioEditor(sf::RenderWindow &window, StateManager &sm) : window(window), stateManager(sm)
{
    options = {
        {"CREATE", [this]()
         {
             currentMode = CREATE;
             levelSelectorState->loadMaps();
         }},
        {"EDIT", [this]()
         { currentMode = EDIT; }},
        {"BACK", [this]()
         {
             stateManager.currentMode = StateManager::MAIN_MENU;
             stateManager.mainMenuState->getHighscore();
             currentMode = MENU;
         }}};

    menu = GameMenu(window.getView(), "SCENARIO\n\nEDITOR", {4, 1}, TextColor::WHITE, options, {9, 9});
    levelSelectorState = std::make_unique<LevelSelectorState>(window, stateManager, *this, "Select Map to Add to Scenario");

    scenario_name = defaultScenarioName();
    writingScenarioName = false;
    textCursorPos = scenario_name.size();

    initScenarioList();
}

void ScenarioEditor::initScenarioList()
{
    scenarios.clear();
    std::string directoryPath = "../../Tappa_15/resources/scenarios/";
    for (const auto &entry : std::filesystem::directory_iterator(directoryPath))
    {
        if (entry.is_regular_file())
        {
            std::string fileName = entry.path().stem().string();
            if(fileName == "classic")
                continue;

            scenarios.push_back({fileName, [this, fileName]()
                                 {
                                     scenario_name = fileName;
                                     currentMode = CREATE;
                                     loadScenario(scenario_name);
                                     levelSelectorState->loadMaps();
                                 }});
        }
    }

    scenarios.push_back({"BACK", [this]()
                         { currentMode = MENU; }});

    scenariosList = GameMenu(window.getView(), "EDIT\n\nSCENARIO", {4, 1}, TextColor::WHITE, scenarios, {9, 9});
}

void ScenarioEditor::doGraphics()
{
    switch (currentMode)
    {
    case MENU:
        menu.draw(window);
        break;
    case CREATE:
        levelSelectorState->draw();
        break;
    case EDIT:
        scenariosList.draw(window);
        break;
    case SAVE_PROMPT:
        levelSelectorState->draw();
        drawSavePrompt();
        break;
    default:
        std::cerr << "Unknown mode (" << currentMode << ")" << std::endl;
        break;
    }
}

void ScenarioEditor::drawSavePrompt()
{
    float promptWidth = 600.f;
    float promptHeight = 200.f;
    sf::RectangleShape promptBox(sf::Vector2f(promptWidth, promptHeight));
    promptBox.setFillColor(sf::Color(0, 0, 0, 200));
    promptBox.setOutlineColor(sf::Color::White);
    promptBox.setOutlineThickness(2.f);
    sf::Vector2f center(
        window.getView().getCenter().x - promptWidth / 2.f,
        window.getView().getCenter().y - promptHeight / 2.f);
    promptBox.setPosition(center);

    window.draw(promptBox);

    std::string saveText = "Save " + scenario_name;
    sf::Vector2f viewCenter = window.getView().getCenter();
    sf::Vector2f viewSize = window.getView().getSize();

    float saveTextX = viewCenter.x - ((saveText.length() / 2.f) * TILE_SIZE);
    float saveTextY = viewCenter.y - 90.f;
    arcadeText.drawString(saveText, saveTextX / TILE_SIZE, saveTextY / TILE_SIZE, window, TextColor::WHITE);

    if (writingScenarioName)
    {
        float cursorX = saveTextX + (5 * TILE_SIZE) + (textCursorPos * TILE_SIZE);
        float cursorY = saveTextY;
        sf::RectangleShape cursor(sf::Vector2f(2.f, TILE_SIZE));
        cursor.setFillColor(sf::Color::White);
        cursor.setPosition({cursorX, cursorY});
        window.draw(cursor);
    }

    std::array line = {
        sf::Vertex{sf::Vector2f(saveTextX + (5 * TILE_SIZE), saveTextY + TILE_SIZE + 5.f)},
        sf::Vertex{sf::Vector2f(saveTextX + saveText.length() * TILE_SIZE, saveTextY + TILE_SIZE + 5.f)}};

    window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);

    sf::RectangleShape checkbox(sf::Vector2f(TILE_SIZE, TILE_SIZE));

    if (loopScenario)
        checkbox.setFillColor(sf::Color::White);
    else
        checkbox.setFillColor(sf::Color::Transparent);

    checkbox.setOutlineColor(sf::Color::White);
    checkbox.setOutlineThickness(2.f);
    checkbox.setPosition({viewCenter.x - 210.f, viewCenter.y - 20.f});
    window.draw(checkbox);

    arcadeText.drawString("Loop Scenario", (viewCenter.x - 170.f) / TILE_SIZE, (viewCenter.y - 17.f) / TILE_SIZE, window, TextColor::WHITE);

    float optionY = viewCenter.y + 50.f;
    float yesTextX = viewCenter.x - 60.f;
    float noTextX = viewCenter.x + 40.f;

    arcadeText.drawString("Y", yesTextX / TILE_SIZE, optionY / TILE_SIZE, window, TextColor::WHITE);
    arcadeText.drawString("N", noTextX / TILE_SIZE, optionY / TILE_SIZE, window, TextColor::WHITE);
}

bool ScenarioEditor::isInScenario(const std::string &mapPath)
{
    return std::find(scenario_list.begin(), scenario_list.end(), mapPath) != scenario_list.end();
}

void ScenarioEditor::addToScenario(std::string mapPath)
{
    if (!isInScenario(mapPath))
    {
        scenario_list.push_back(mapPath);
        std::cout << "Added " << mapPath << " to scenario list." << std::endl;
    }
    else
    {
        std::cout << mapPath << " is already in the scenario list." << std::endl;
    }
}

void ScenarioEditor::removeFromScenario(std::string mapPath)
{
    auto it = std::find(scenario_list.begin(), scenario_list.end(), mapPath);
    if (it != scenario_list.end())
    {
        scenario_list.erase(it);
        std::cout << "Removed " << mapPath << " from scenario list." << std::endl;
    }
    else
    {
        std::cout << mapPath << " is not in the scenario list." << std::endl;
    }
}

void ScenarioEditor::saveScenario(const std::string &scenarioName)
{
    std::ofstream scenarioFile;
    std::string safeScenarioName = scenarioName;
    std::replace(safeScenarioName.begin(), safeScenarioName.end(), ' ', '_');
    if (safeScenarioName.empty())
    {
        safeScenarioName = defaultScenarioName();
    }
    std::string filePath = "../../Tappa_15/resources/scenarios/" + safeScenarioName + ".txt";
    scenarioFile.open(filePath, std::ios::out);
    if (!scenarioFile.is_open())
    {
        std::cerr << "Error opening scenario file for writing: " << filePath << std::endl;
        return;
    }

    scenarioFile << (loopScenario ? "1\n" : "0\n");
    for (const auto &mapName : scenario_list)
    {
        scenarioFile << mapName << ",";
    }

    scenarioFile.close();
    scenario_list.clear();
    scenario_name = defaultScenarioName();
    textCursorPos = scenario_name.size();
}

void ScenarioEditor::loadScenario(const std::string &scenarioName)
{
    scenario_list.clear();
    std::string directoryPath = "../../Tappa_15/resources/scenarios/";
    std::ifstream scenarioFile(directoryPath + scenarioName + ".txt");

    if (!scenarioFile.is_open())
    {
        std::cerr << "Error opening scenario file: " << scenarioName << std::endl;
        return;
    }

    std::string line;
    if (getline(scenarioFile, line))
    {
        loopScenario = (line == "1");
    }


    while (getline(scenarioFile, line, ','))
    {
        scenario_list.push_back(line);
    }
}

std::string ScenarioEditor::defaultScenarioName()
{
    int scenarioCount = 0;
    std::string baseName = "scenario";
    std::string directoryPath = "../../Tappa_15/resources/scenarios/";

    for (const auto &entry : std::filesystem::directory_iterator(directoryPath))
    {
        if (entry.is_regular_file())
        {
            std::string fileName = entry.path().stem().string();
            if (fileName.rfind(baseName, 0) == 0)
            {
                std::string numberStr = fileName.substr(baseName.size());
                try
                {
                    int number = std::stoi(numberStr);
                    if (number > scenarioCount)
                    {
                        scenarioCount = number;
                    }
                }
                catch (const std::exception &e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
        }
    }

    return baseName + std::to_string(scenarioCount + 1);
}

int ScenarioEditor::getScenarioSize(std::string& scenarioName) {
    std::string directoryPath = "../../Tappa_15/resources/scenarios/";
    std::ifstream scenarioFile(directoryPath + scenarioName + ".txt");

    if (!scenarioFile.is_open())
    {
        std::cerr << "Error opening scenario file: " << scenarioName << std::endl;
        return 0;
    }

    std::string line;
    if (getline(scenarioFile, line)) {} // Loop

    int count = 0;
    while (getline(scenarioFile, line, ','))
    {
        count++;
    }

    return count;
}

void ScenarioEditor::handle(const sf::Event::KeyPressed &key)
{
    if (writingScenarioName)
    {
        if (key.scancode == sf::Keyboard::Scancode::Enter || key.scancode == sf::Keyboard::Scancode::Escape)
        {
            writingScenarioName = false;
        }
        else if (key.scancode == sf::Keyboard::Scancode::Left)
        {
            if (textCursorPos > 0)
                textCursorPos--;
        }
        else if (key.scancode == sf::Keyboard::Scancode::Right)
        {
            if (textCursorPos < scenario_name.size())
                textCursorPos++;
        }
    }
    else
    {
        if (key.scancode == sf::Keyboard::Scancode::Y)
        {
            saveScenario(scenario_name);
            currentMode = ScenarioEditor::MENU;
        }
        else if (key.scancode == sf::Keyboard::Scancode::N)
        {
            currentMode = ScenarioEditor::MENU;
        }
    }
}

void ScenarioEditor::handle(const sf::Event::TextEntered &textEntered)
{
    if (writingScenarioName)
    {
        if (textEntered.unicode == '\b')
        {
            if (textCursorPos == 0)
                return;
            textCursorPos--;
            if (textCursorPos < scenario_name.size())
                scenario_name.erase(textCursorPos, 1);
        }
        else if (scenario_name.size() < 13)
        {
            if (std::isalnum(textEntered.unicode) || textEntered.unicode == '!' || textEntered.unicode == '-' || textEntered.unicode == ' ')
            {
                scenario_name.insert(textCursorPos, 1, static_cast<char>(textEntered.unicode));
                textCursorPos++;
            }
        }
    }
}

void ScenarioEditor::handle(const sf::Event::MouseButtonPressed &mousePressed)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if (currentMode == SAVE_PROMPT)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

            std::string saveText = "Save " + scenario_name;
            sf::Vector2f viewCenter = window.getView().getCenter();
            float nameBoxX = viewCenter.x - ((saveText.length() / 2.f) * TILE_SIZE);
            float nameBoxY = viewCenter.y - 90.f;
            float nameBoxWidth = (saveText.length() - 5) * TILE_SIZE;
            float nameBoxHeight = TILE_SIZE + 5.f;

            sf::FloatRect nameBox(
                {nameBoxX + (5 * TILE_SIZE), nameBoxY},
                {nameBoxWidth, nameBoxHeight});

            if (nameBox.contains(worldPos))
            {
                writingScenarioName = true;
                textCursorPos = scenario_name.size();
            }
            else
            {
                writingScenarioName = false;

                sf::FloatRect loopBox({viewCenter.x - 210.f, viewCenter.y - 20.f}, {TILE_SIZE * 14.f, TILE_SIZE});
                if (loopBox.contains(worldPos))
                {
                    loopScenario = !loopScenario;
                }
                else
                {
                    float yesTextX = viewCenter.x - 60.f;
                    float noTextX = viewCenter.x + 40.f;

                    sf::FloatRect yesBox({yesTextX, viewCenter.y + 50.f}, {TILE_SIZE, TILE_SIZE});
                    sf::FloatRect noBox({noTextX, viewCenter.y + 50.f}, {TILE_SIZE, TILE_SIZE});

                    if (yesBox.contains(worldPos))
                    {
                        saveScenario(scenario_name);
                        currentMode = ScenarioEditor::MENU;
                    }
                    else if (noBox.contains(worldPos))
                    {
                        currentMode = ScenarioEditor::MENU;
                    }
                }
            }
        }
    }
}