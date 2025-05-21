#pragma once

inline const int MAP_WIDTH = 28;  // area di gioco
inline const int MAP_HEIGHT = 31; // area di gioco
inline const int TILE_SIZE = 32;
inline const int TEXT_SIZE = 8;

inline const char PACDOT = '0';
inline const char LINE_V = '1';
inline const char LINE_H = '2';
inline const char CORNER_0 = '3';
inline const char CORNER_90 = '4';
inline const char CORNER_180 = '5';
inline const char CORNER_270 = '6';
inline const char POWERPELLET = '7';
inline const char GHOST_DOOR = '8';
inline const char EMPTY_BLOCK = '9';

inline const char PACMAN = 'P';
inline const char BLINKY = 'B';
inline const char PINKY = 'N';
inline const char INKY = 'I';
inline const char CLYDE = 'C';

inline const char FRUIT = 'F';

enum Direction
{
    UP = 1,
    DOWN = 3,
    LEFT = 0,
    RIGHT = 2,
    NONE = 4
};