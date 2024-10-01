#include "gameObject.cpp"

extern const int WINDOW_WIDTH, WINDOW_HEIGHT;

class starshipGameObj : public gameObject
{
    public:
        int m_health = 3;

        starshipGameObj(std::string name, sf::Texture* texture) : gameObject (name)
        {
            m_sprite.setTexture(*texture);
            m_sprite.setPosition( WINDOW_WIDTH/2, WINDOW_HEIGHT-30);
        }
};