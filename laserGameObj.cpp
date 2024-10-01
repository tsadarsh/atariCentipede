#include "gameObject.cpp"

class laserGameObj : public gameObject
{
    public: 
        laserGameObj (std::string name, sf::Texture texture) : gameObject (name)
        {
            m_sprite.setTexture(texture);
        }
};