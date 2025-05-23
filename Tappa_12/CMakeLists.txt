cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main 
    src/core/Animation.cpp
    src/core/Debug.cpp
    src/core/StateManager.cpp

    src/lib/TextUI.cpp
    src/lib/textures.cpp
    src/lib/TileFactory.cpp

    src/game_elements/Fruit.cpp
    src/game_elements/GameState.cpp
    src/game_elements/Ghost.cpp
    src/game_elements/PacMan.cpp
    src/game_elements/fruits/Apple.cpp
    src/game_elements/fruits/Bell.cpp
    src/game_elements/fruits/Cherry.cpp
    src/game_elements/fruits/Galaxian.cpp
    src/game_elements/fruits/Grape.cpp
    src/game_elements/fruits/Key.cpp
    src/game_elements/fruits/Orange.cpp
    src/game_elements/fruits/Strawberry.cpp
    src/game_elements/ghosts/Blinky.cpp
    src/game_elements/ghosts/Clyde.cpp
    src/game_elements/ghosts/Inky.cpp
    src/game_elements/ghosts/Pinky.cpp

    src/map_editor/MapEditor.cpp
    src/map_editor/Create.cpp

    src/main.cpp
    src/GameMenu.cpp
    src/LevelSelectorState.cpp
    src/MainMenuState.cpp
)

target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics)