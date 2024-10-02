#include "gameObject.cpp"
#include "helper_func.cpp"

class centipedeGameObject : public gameObject
{
    public:
        std::vector<gameObject> centipede;
        float speedX = -0.1;
        float moveY = 40;
        sf::Texture *m_textureHead;
        sf::Texture *m_textureBody;
        sf::Sprite ECE_Centipede;

        centipedeGameObject(std::string name, sf::Texture *textureHead, sf::Texture *textureBody) : gameObject (name) 
        {
            m_textureHead = textureHead;
            m_textureBody = textureBody;
        }

        void spawn(int lengthOfCentipede, float spawnX, float spawnY)
        {
            if (lengthOfCentipede > 0)
            {
                ECE_Centipede = m_sprite;
                std::cout << "Generate: " << lengthOfCentipede << std::endl;
                gameObject centipedeHead("head");
                centipedeHead.m_sprite.setTexture(*m_textureHead);
                centipedeHead.setPosition(spawnX, spawnY);
                centipede.push_back(centipedeHead);
                for(int i_body = 1; i_body < lengthOfCentipede; i_body++)
                {
                    gameObject centipedeBody("body" + std::to_string(i_body));
                    centipedeBody.m_sprite.setTexture(*m_textureBody);
                    centipedeBody.setPosition(spawnX + (i_body+1)*20, spawnY);
                    centipede.push_back(centipedeBody);
                }
            }
        }

        void move(float offsetX, float offsetY, float deltaTime)
        {
            centipede[0].move(offsetX, offsetY);
            for (int i_body=1; i_body < centipede.size(); i_body++)
            {
                sf::Vector2f dir = centipede[i_body-1].m_sprite.getPosition() - centipede[i_body].m_sprite.getPosition() ;
                float dist = distance(centipede[i_body-1].m_sprite.getPosition(), centipede[i_body].m_sprite.getPosition());

                if (dist > 20)
                {
                    centipede[i_body].move((normalize(dir) * (200 * deltaTime)).x, (normalize(dir) * (200 * deltaTime)).y);
                }
            }

        }
};