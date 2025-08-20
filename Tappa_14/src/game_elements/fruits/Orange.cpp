#include "../../../includes/game_elements/fruits/Orange.hpp"

Orange::Orange(sf::Vector2i position, GameState &gameState)
    : Fruit(position, ORANGE_TEX, 500, gameState) {}