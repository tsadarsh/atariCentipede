#include "gameObject.cpp"

class mushroomGameObj : public gameObject
{
    public:
        int health = 2;
        sf::Texture* m_textureHealth2;
        sf::Texture* m_textureHealth1;

        mushroomGameObj(std::string name, sf::Texture *textureHealth2, sf::Texture *textureHealth1) : gameObject(name) 
        {
            m_textureHealth2 = textureHealth2;
            m_textureHealth1 = textureHealth1;
            m_sprite.setTexture(*textureHealth2);
        }

        void damage(int pts)
        {
            health--;
            switch (health)
            {
            case 2:
                m_sprite.setTexture(*m_textureHealth2);
                break;
            
            case 1:
                m_sprite.setTexture(*m_textureHealth1);
                break;
            
            case 0:
                setPosition(-10, -10); // mushroom positioned out of window
                break;
            
            default:
                break;
            }
        }
};
