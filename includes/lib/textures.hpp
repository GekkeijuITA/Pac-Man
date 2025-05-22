#pragma once

#include <SFML/Graphics.hpp>
#include <string>

inline const std::string STRAIGHT_LINE_H = "../resources/tiles/straight-tile-h.jpg";
inline const std::string ANGLE_0 = "../resources/tiles/corner-tile-0.jpg";
inline const std::string PACDOT_TEX = "../resources/tiles/pacdot.png";
inline const std::string GHOST_DOOR_TEX_H = "../resources/tiles/ghostdoor.png";
inline const std::string ASSET = "../resources/asset.png";
inline const std::string TEXT = "../resources/ui/text.png";

sf::Sprite createSprite(const sf::Texture &texture,
                        const sf::Vector2i &position,
                        const sf::Vector2f &scale,
                        float scaleFactor,
                        int tileSize,
                        bool withOrigin,
                        int offset = 0,
                        float extraWidth = 0.f);