#include "../includes/MapEditorState.hpp"
#include "../includes/Debug.hpp"

MapEditorState::MapEditorState(sf::RenderWindow &window) : window(window)
{
    menu = GameMenu(window.getView(), "MAP EDITOR", options, {4, 1});
}

void MapEditorState::doGraphics()
{
    menu.draw(window);

    // Debug::drawGrid(window);
}