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
    src/Animation.cpp
    src/Debug.cpp
    src/Fruit.cpp
    src/GameMenu.cpp
    src/GameState.cpp
    src/Ghost.cpp
    src/main.cpp
    src/MainMenuState.cpp
    src/PacMan.cpp
    src/StateManager.cpp
    src/TextUI.cpp
    src/textures.cpp
    src/LevelSelectorState.cpp
    src/fruits/Apple.cpp
    src/fruits/Bell.cpp
    src/fruits/Cherry.cpp
    src/fruits/Galaxian.cpp
    src/fruits/Grape.cpp
    src/fruits/Key.cpp
    src/fruits/Orange.cpp
    src/fruits/Strawberry.cpp
    src/ghosts/Blinky.cpp
    src/ghosts/Clyde.cpp
    src/ghosts/Inky.cpp
    src/ghosts/Pinky.cpp
)

target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics)