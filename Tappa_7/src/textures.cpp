#include "../includes/textures.hpp"
#include "../includes/global_values.hpp"

sf::Sprite createSprite(
    const sf::Texture &texture,
    const sf::Vector2i &position,
    const sf::Vector2f &scale,
    float scaleFactor,
    int tileSize = TILE_SIZE,
    bool withOrigin = true)
{
    sf::IntRect texRect({position.x * tileSize, position.y * tileSize},
                        {tileSize, tileSize});

    sf::Sprite pacmanSprite(texture, texRect);
    if (withOrigin)
        pacmanSprite.setOrigin({tileSize / 2.f, tileSize / 2.f});
    pacmanSprite.setScale(scale * scaleFactor);

    return pacmanSprite;
}