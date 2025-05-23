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

target_include_directories(Tappa1 PRIVATE
    "${CMAKE_SOURCE_DIR}/includes"
)

target_link_libraries(Tappa1 PRIVATE SFML::Graphics)