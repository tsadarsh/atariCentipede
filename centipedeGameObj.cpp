#include "gameObject.cpp"
#include "helper_func.cpp"

class centipedeGameObject : public gameObject
{
    public:
        std::vector<gameObject> centipede;
        centipedeGameObject() : gameObject () {}
        float speedX = 0.1;
        float moveY = 40;

        void spawn(int lengthOfCentipede, float spawnX, float spawnY)
        {
            gameObject centipedeHead("centipedeHead", "/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/CentipedeHead.png", spawnX, spawnY);
            centipede.push_back(centipedeHead);
            for(int i=0; i<lengthOfCentipede; i++)
            {
                gameObject centipedeBody("centipedeBody"+std::to_string(i), "/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/CentipedeBody.png", spawnX, spawnY + (i+1)*20);
                centipede.push_back(centipedeBody);
            }
            centipede[0].updateTexture("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/CentipedeHead.png");
            centipede[0].sprite.setPosition(400, 300);
            for(int i=1;i<lengthOfCentipede;i++)
            {
                centipede[i].updateTexture("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/CentipedeBody.png");
                centipede[i].sprite.setPosition(400, 300 + (i+1)*20);
            }
        }

        void move(float offsetX, float offsetY, float deltaTime)
        {
            centipede[0].move(offsetX, offsetY);
            for (int i_body=1; i_body < centipede.size(); i_body++)
            {
                sf::Vector2f dir = centipede[i_body-1].sprite.getPosition() - centipede[i_body].sprite.getPosition() ;
                float dist = distance(centipede[i_body-1].sprite.getPosition(), centipede[i_body].sprite.getPosition());

                if (dist > 20)
                {
                    centipede[i_body].move((normalize(dir) * (200 * deltaTime)).x, (normalize(dir) * (200 * deltaTime)).y);
                }
            }

        }
};