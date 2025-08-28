#pragma once

#include <SFML/Graphics.hpp>
#include <string>

inline const std::string STRAIGHT_LINE_H = "../../Tappa_14/resources/tiles/straight-tile-h.jpg";
inline const std::string STRAIGHT_LINE_H_BLINK = "../../Tappa_14/resources/tiles/straight-tile-h-blink.png";
inline const std::string ANGLE_0 = "../../Tappa_14/resources/tiles/corner-tile-0.jpg";
inline const std::string ANGLE_0_BLINK = "../../Tappa_14/resources/tiles/corner-tile-0-blink.png";
inline const std::string PACDOT_TEX = "../../Tappa_14/resources/tiles/pacdot.png";
inline const std::string POWERPELLET_TEX = "../../Tappa_14/resources/tiles/powerpellet.png";
inline const std::string GHOST_DOOR_TEX_H = "../../Tappa_14/resources/tiles/ghostdoor.png";
inline const std::string ASSET = "../../Tappa_14/resources/asset.png";
inline const std::string TEXT = "../../Tappa_14/resources/ui/text.png";

sf::Sprite createSprite(const sf::Texture &texture,
                        const sf::Vector2i &position,
                        const sf::Vector2f &scale,
                        float scaleFactor,
                        int tileSize,
                        bool withOrigin,
                        int offset = 0,
                        float extraWidth = 0.f);