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

target_link_libraries(Tappa1 PRIVATE SFML::Graphics)
target_link_libraries(Tappa2 PRIVATE SFML::Graphics)
target_link_libraries(Tappa3 PRIVATE SFML::Graphics)
target_link_libraries(Tappa4 PRIVATE SFML::Graphics)
target_link_libraries(Tappa5 PRIVATE SFML::Graphics)
target_link_libraries(Tappa6 PRIVATE SFML::Graphics)
target_link_libraries(Tappa7 PRIVATE SFML::Graphics)