#include "../../includes/game_elements/Fruit.hpp"
#include "../../includes/lib/TileFactory.hpp"

Fruit::Fruit(
    sf::Vector2i position,
    sf::Vector2i texPosition,
    int score) : position(position), score(score)
{
    if (!tex.loadFromFile(ASSET))
    {
        std::cerr << "Errore nel caricamento della texture del fantasma" << std::endl;
        exit(1);
    }

    fruitDisplayTimer = 0.f;
    scoreDisplayTimer = SCORE_DISPLAY_TIME;
}

int Fruit::getScore()
{
    return score;
}

void Fruit::draw(sf::RenderWindow &window, char tileType)
{
    auto tile = TileFactory::getIstance().getTile(tileType);
    if (!tile)
    {
        std::cerr << "Error loading tile data" << std::endl;
        return;
    }

    float x = static_cast<float>((position.y + .5f) * TILE_SIZE);
    float y = static_cast<float>((position.x + 3.5f) * TILE_SIZE);
    tile->sprite.setPosition({x, y});
    tile->sprite.setScale({3.f, 3.f});
    window.draw(tile->sprite);
}

// Imposta il timer casualmente tra 9 e 10 secondi
void Fruit::setTimer()
{
    fruitDisplayTimer = fruitDisplayTimer = (rand() % 2) + 9;
}

void Fruit::drawScore(sf::RenderWindow &window)
{
    sf::Vector2i scorePos;

    switch (score)
    {
    case 100:
        scorePos = FRUIT_SCORE_100;
        break;
    case 300:
        scorePos = FRUIT_SCORE_300;
        break;
    case 500:
        scorePos = FRUIT_SCORE_500;
        break;
    case 700:
        scorePos = FRUIT_SCORE_700;
        break;
    case 1000:
        scorePos = FRUIT_SCORE_1000;
        break;
    case 2000:
        scorePos = FRUIT_SCORE_2000;
        break;
    case 3000:
        scorePos = FRUIT_SCORE_3000;
        break;
    case 5000:
        scorePos = FRUIT_SCORE_5000;
        break;
    default:
        return;
    }

    int offset = 0;
    float extraWidth = 0.f;
    if (score == 1000)
    {
        offset = 1;
        extraWidth = 3.f;
    }
    else if (score > 1000)
    {
        offset = 3;
        extraWidth = 2.f * offset;
    }

    sf::Sprite sprite = createSprite(tex, scorePos, {2.f, 2.f}, 1.5f, TILE_SIZE / 2, true, offset, extraWidth);
    sprite.setPosition({(position.y + .5f) * TILE_SIZE, (position.x + 3.5f) * TILE_SIZE});
    window.draw(sprite);
}