#include <SFML/Graphics.hpp>

class PacMan
{
    public:
        float speed;
        sf::Texture tex;
        sf::Vector2u position;
        int direction; // 0=sinistra,1=su,2=destra,3=giù

        PacMan();
        void draw(sf::RenderWindow &window);
        void move(float x, float y);
};