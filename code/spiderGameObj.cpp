#include "gameObject.cpp"

class spiderGameObj : public gameObject
{
    public:
        float targetX;
        float targetY;
        float speed;
        const int NEW_TARGET_POS_DELTA = 400;
    
        spiderGameObj(std::string name, sf::Texture *texture) : gameObject(name) 
        {
            m_sprite.setTexture(*texture);
        }
};