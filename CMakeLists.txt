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

message(STATUS "Include directory: ${CMAKE_SOURCE_DIR}/includes")

#[[
add_executable(
    Tappa1
     
    Tappa_1/src/main.cpp
)

add_executable(
    Tappa2 

    Tappa_2/src/main.cpp
)

add_executable(
    Tappa3 

    Tappa_3/src/main.cpp
    Tappa_3/src/PacMan.cpp
)

add_executable(
    Tappa4

    Tappa_4/src/main.cpp
    Tappa_4/src/PacMan.cpp
    Tappa_4/src/textures.cpp
)

add_executable(
    Tappa5 

    Tappa_5/src/main.cpp
    Tappa_5/src/PacMan.cpp
    Tappa_5/src/textures.cpp
    Tappa_5/src/Ghost.cpp

    Tappa_5/src/ghosts/Blinky.cpp
    Tappa_5/src/ghosts/Clyde.cpp
    Tappa_5/src/ghosts/Inky.cpp
    Tappa_5/src/ghosts/Pinky.cpp
)

add_executable(
    Tappa6 

    Tappa_6/src/main.cpp
    Tappa_6/src/PacMan.cpp
    Tappa_6/src/textures.cpp
    Tappa_6/src/Ghost.cpp
    Tappa_6/src/State.cpp

    Tappa_6/src/ghosts/Blinky.cpp
    Tappa_6/src/ghosts/Clyde.cpp
    Tappa_6/src/ghosts/Inky.cpp
    Tappa_6/src/ghosts/Pinky.cpp
)

add_executable(
    Tappa7 

    Tappa_7/src/main.cpp
    Tappa_7/src/PacMan.cpp
    Tappa_7/src/textures.cpp
    Tappa_7/src/Ghost.cpp
    Tappa_7/src/State.cpp

    Tappa_7/src/ghosts/Blinky.cpp
    Tappa_7/src/ghosts/Clyde.cpp
    Tappa_7/src/ghosts/Inky.cpp
    Tappa_7/src/ghosts/Pinky.cpp
)

add_executable(
    Tappa8 

    Tappa_8/src/main.cpp
    Tappa_8/src/PacMan.cpp
    Tappa_8/src/textures.cpp
    Tappa_8/src/Ghost.cpp
    Tappa_8/src/State.cpp
    Tappa_8/src/Fruit.cpp

    Tappa_8/src/ghosts/Blinky.cpp
    Tappa_8/src/ghosts/Clyde.cpp
    Tappa_8/src/ghosts/Inky.cpp
    Tappa_8/src/ghosts/Pinky.cpp
  
    Tappa_8/src/fruits/Cherry.cpp
    Tappa_8/src/fruits/Strawberry.cpp
    Tappa_8/src/fruits/Orange.cpp
    Tappa_8/src/fruits/Apple.cpp
    Tappa_8/src/fruits/Grape.cpp
    Tappa_8/src/fruits/Galaxian.cpp
    Tappa_8/src/fruits/Key.cpp
    Tappa_8/src/fruits/Bell.cpp
)

add_executable(
    Tappa9 

    Tappa_9/src/Debug.cpp
    Tappa_9/src/Fruit.cpp
    Tappa_9/src/GameMenu.cpp
    Tappa_9/src/GameState.cpp
    Tappa_9/src/Ghost.cpp
    Tappa_9/src/main.cpp
    Tappa_9/src/MainMenuState.cpp
    Tappa_9/src/PacMan.cpp
    Tappa_9/src/StateManager.cpp
    Tappa_9/src/TextUI.cpp
    Tappa_9/src/textures.cpp

    Tappa_9/src/ghosts/Blinky.cpp
    Tappa_9/src/ghosts/Clyde.cpp
    Tappa_9/src/ghosts/Inky.cpp
    Tappa_9/src/ghosts/Pinky.cpp
  
    Tappa_9/src/fruits/Cherry.cpp
    Tappa_9/src/fruits/Strawberry.cpp
    Tappa_9/src/fruits/Orange.cpp
    Tappa_9/src/fruits/Apple.cpp
    Tappa_9/src/fruits/Grape.cpp
    Tappa_9/src/fruits/Galaxian.cpp
    Tappa_9/src/fruits/Key.cpp
    Tappa_9/src/fruits/Bell.cpp
)

add_executable(
    Tappa10 

    Tappa_10/src/Animation.cpp
    Tappa_10/src/Debug.cpp
    Tappa_10/src/Fruit.cpp
    Tappa_10/src/GameMenu.cpp
    Tappa_10/src/GameState.cpp
    Tappa_10/src/Ghost.cpp
    Tappa_10/src/main.cpp
    Tappa_10/src/MainMenuState.cpp
    Tappa_10/src/PacMan.cpp
    Tappa_10/src/StateManager.cpp
    Tappa_10/src/TextUI.cpp
    Tappa_10/src/textures.cpp

    Tappa_10/src/ghosts/Blinky.cpp
    Tappa_10/src/ghosts/Clyde.cpp
    Tappa_10/src/ghosts/Inky.cpp
    Tappa_10/src/ghosts/Pinky.cpp
  
    Tappa_10/src/fruits/Cherry.cpp
    Tappa_10/src/fruits/Strawberry.cpp
    Tappa_10/src/fruits/Orange.cpp
    Tappa_10/src/fruits/Apple.cpp
    Tappa_10/src/fruits/Grape.cpp
    Tappa_10/src/fruits/Galaxian.cpp
    Tappa_10/src/fruits/Key.cpp
    Tappa_10/src/fruits/Bell.cpp
)

add_executable(
    Tappa11 

    Tappa_11/src/Animation.cpp
    Tappa_11/src/Debug.cpp
    Tappa_11/src/Fruit.cpp
    Tappa_11/src/GameMenu.cpp
    Tappa_11/src/GameState.cpp
    Tappa_11/src/Ghost.cpp
    Tappa_11/src/main.cpp
    Tappa_11/src/MainMenuState.cpp
    Tappa_11/src/PacMan.cpp
    Tappa_11/src/StateManager.cpp
    Tappa_11/src/TextUI.cpp
    Tappa_11/src/textures.cpp
    Tappa_11/src/LevelSelectorState.cpp

    Tappa_11/src/ghosts/Blinky.cpp
    Tappa_11/src/ghosts/Clyde.cpp
    Tappa_11/src/ghosts/Inky.cpp
    Tappa_11/src/ghosts/Pinky.cpp
  
    Tappa_11/src/fruits/Cherry.cpp
    Tappa_11/src/fruits/Strawberry.cpp
    Tappa_11/src/fruits/Orange.cpp
    Tappa_11/src/fruits/Apple.cpp
    Tappa_11/src/fruits/Grape.cpp
    Tappa_11/src/fruits/Galaxian.cpp
    Tappa_11/src/fruits/Key.cpp
    Tappa_11/src/fruits/Bell.cpp
)

add_executable(
    Tappa12 

    Tappa_12/src/core/Animation.cpp
    Tappa_12/src/core/Debug.cpp
    Tappa_12/src/core/StateManager.cpp

    Tappa_12/src/lib/TextUI.cpp
    Tappa_12/src/lib/textures.cpp
    Tappa_12/src/lib/TileFactory.cpp

    Tappa_12/src/game_elements/Fruit.cpp
    Tappa_12/src/game_elements/GameState.cpp
    Tappa_12/src/game_elements/Ghost.cpp
    Tappa_12/src/game_elements/PacMan.cpp
    Tappa_12/src/game_elements/fruits/Apple.cpp
    Tappa_12/src/game_elements/fruits/Bell.cpp
    Tappa_12/src/game_elements/fruits/Cherry.cpp
    Tappa_12/src/game_elements/fruits/Galaxian.cpp
    Tappa_12/src/game_elements/fruits/Grape.cpp
    Tappa_12/src/game_elements/fruits/Key.cpp
    Tappa_12/src/game_elements/fruits/Orange.cpp
    Tappa_12/src/game_elements/fruits/Strawberry.cpp
    Tappa_12/src/game_elements/ghosts/Blinky.cpp
    Tappa_12/src/game_elements/ghosts/Clyde.cpp
    Tappa_12/src/game_elements/ghosts/Inky.cpp
    Tappa_12/src/game_elements/ghosts/Pinky.cpp

    Tappa_12/src/map_editor/MapEditor.cpp
    Tappa_12/src/map_editor/Create.cpp

    Tappa_12/src/main.cpp
    Tappa_12/src/GameMenu.cpp
    Tappa_12/src/LevelSelectorState.cpp
    Tappa_12/src/MainMenuState.cpp
)]]#

add_executable(
    Tappa13

    Tappa_13/src/core/Animation.cpp
    Tappa_13/src/core/Debug.cpp
    Tappa_13/src/core/StateManager.cpp

    Tappa_13/src/lib/TextUI.cpp
    Tappa_13/src/lib/textures.cpp
    Tappa_13/src/lib/TileFactory.cpp

    Tappa_13/src/game_elements/Fruit.cpp
    Tappa_13/src/game_elements/GameState.cpp
    Tappa_13/src/game_elements/Ghost.cpp
    Tappa_13/src/game_elements/PacMan.cpp
    Tappa_13/src/game_elements/fruits/Apple.cpp
    Tappa_13/src/game_elements/fruits/Bell.cpp
    Tappa_13/src/game_elements/fruits/Cherry.cpp
    Tappa_13/src/game_elements/fruits/Galaxian.cpp
    Tappa_13/src/game_elements/fruits/Grape.cpp
    Tappa_13/src/game_elements/fruits/Key.cpp
    Tappa_13/src/game_elements/fruits/Orange.cpp
    Tappa_13/src/game_elements/fruits/Strawberry.cpp
    Tappa_13/src/game_elements/ghosts/Blinky.cpp
    Tappa_13/src/game_elements/ghosts/Clyde.cpp
    Tappa_13/src/game_elements/ghosts/Inky.cpp
    Tappa_13/src/game_elements/ghosts/Pinky.cpp

    Tappa_13/src/map_editor/MapEditor.cpp
    Tappa_13/src/map_editor/Create.cpp

    Tappa_13/src/main.cpp
    Tappa_13/src/GameMenu.cpp
    Tappa_13/src/LevelSelectorState.cpp
    Tappa_13/src/MainMenuState.cpp
)

#[[target_link_libraries(Tappa1 PRIVATE SFML::Graphics)
target_link_libraries(Tappa2 PRIVATE SFML::Graphics)
target_link_libraries(Tappa3 PRIVATE SFML::Graphics)
target_link_libraries(Tappa4 PRIVATE SFML::Graphics)
target_link_libraries(Tappa5 PRIVATE SFML::Graphics)
target_link_libraries(Tappa6 PRIVATE SFML::Graphics)
target_link_libraries(Tappa7 PRIVATE SFML::Graphics)
target_link_libraries(Tappa8 PRIVATE SFML::Graphics)
target_link_libraries(Tappa9 PRIVATE SFML::Graphics)
target_link_libraries(Tappa10 PRIVATE SFML::Graphics)
target_link_libraries(Tappa11 PRIVATE SFML::Graphics)
target_link_libraries(Tappa12 PRIVATE SFML::Graphics)]]#
target_link_libraries(Tappa13 PRIVATE SFML::Graphics)