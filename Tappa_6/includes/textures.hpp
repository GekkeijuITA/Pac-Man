#pragma once

#include <SFML/Graphics.hpp>
#include <string>

inline const std::string STRAIGHT_LINE_H = "../Tappa_6/resources/tiles/straight-tile-h.jpg";
inline const std::string ANGLE_0 = "../Tappa_6/resources/tiles/corner-tile-0.jpg";
inline const std::string ASSET = "../Tappa_6/resources/asset.png";
inline const std::string TEXT = "../Tappa_6/resources/ui/text.png";
sf::Sprite createSprite(const sf::Texture &texture,
                        const sf::Vector2i &position,
                        const sf::Vector2f &scale,
                        float scaleFactor,
                        int tileSize,
                        bool withOrigin
                    );