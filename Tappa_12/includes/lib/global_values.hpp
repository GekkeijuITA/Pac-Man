#pragma once

inline const int MAP_WIDTH = 28;  // area di gioco
inline const int MAP_HEIGHT = 31; // area di gioco
inline const int TILE_SIZE = 32;
inline const int TEXT_SIZE = 8;

inline const char LINE_V = 'V';
inline const char LINE_H = 'H';

inline const char CORNER_0 = '0';
inline const char CORNER_90 = '1';
inline const char CORNER_180 = '2';
inline const char CORNER_270 = '3';
inline const char PACDOT = '4';
inline const char POWERPELLET = '7';
inline const char GHOST_DOOR_H = '8';
inline const char GHOST_DOOR_V = '5';
inline const char EMPTY_BLOCK = '9';

inline const char PACMAN = 'P';
inline const char BLINKY = 'B';
inline const char PINKY = 'N';
inline const char INKY = 'I';
inline const char CLYDE = 'C';

inline const char FRUIT = 'F';

inline const char CHERRY = 'c';
inline const char STRAWBERRY = 's';
inline const char ORANGE_FRUIT = 'o';
inline const char APPLE = 'a';
inline const char GRAPE = 'g';
inline const char GALAXIAN = 'l';
inline const char BELL = 'b';
inline const char KEY = 'k';

enum Direction
{
    UP = 1,
    DOWN = 3,
    LEFT = 0,
    RIGHT = 2,
    NONE = 4
};