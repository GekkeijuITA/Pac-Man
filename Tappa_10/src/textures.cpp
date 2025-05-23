#include "../includes/textures.hpp"
#include "../includes/global_values.hpp"

// Crea uno sprite da un asset di texture
// con la posizione, la scala e il fattore di scala specificati
sf::Sprite createSprite(
    const sf::Texture &texture,
    const sf::Vector2i &position,
    const sf::Vector2f &scale,
    float scaleFactor,
    int tileSize,
    bool withOrigin,
    int offset, 
    float extraWidth)
{
    sf::IntRect texRect({(position.x * tileSize) - offset, position.y * tileSize},
                        {tileSize + extraWidth, tileSize});

    sf::Sprite sprite(texture, texRect);
    if (withOrigin)
        sprite.setOrigin({(tileSize + offset) / 2.f, (tileSize + offset) / 2.f});
    sprite.setScale(scale * scaleFactor);

    return sprite;
}