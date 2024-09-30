#include "gameObject.cpp"
#include "helper_func.cpp"

class centipedeGameObject : public gameObject
{
    public:
        std::vector<std::vector<gameObject>> family;
        centipedeGameObject() : gameObject () {}

        void spawnNew()
        {
            std::vector<gameObject> centipede;
            gameObject centipedeHead("centipedeHead", "/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/CentipedeHead.png", 400, 300);
            centipede.push_back(centipedeHead);
            for(int i=0; i<10; i++)
            {
                gameObject centipedeBody("centipedeBody"+std::to_string(i), "/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/CentipedeBody.png", 400, 300 + (i+1)*20);
                centipede.push_back(centipedeBody);
            }
            centipede[0].updateTexture("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/CentipedeHead.png");
            centipede[0].sprite.setPosition(400, 300);
            for(int i=1;i<11;i++)
            {
                centipede[i].updateTexture("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/CentipedeBody.png");
                centipede[i].sprite.setPosition(400, 300 + (i+1)*20);
            }
            family.push_back(centipede);   
        }

        void move(float offsetX, float offsetY, float deltaTime)
        {
            for(int i_family=0; i_family<family.size(); i_family++)
            {
                family[i_family][0].move(offsetX, offsetY);
                for (int i=1; i < 11; ++i)
                {
                    sf::Vector2f dir = family[i_family][i-1].sprite.getPosition() - family[i_family][i].sprite.getPosition() ;
                    float dist = distance(family[i_family][i-1].sprite.getPosition(), family[i_family][i].sprite.getPosition());

                    if (dist > 20)
                    {
                        family[i_family][i].move((normalize(dir) * (200 * deltaTime)).x, (normalize(dir) * (200 * deltaTime)).y);
                    }
                }
            }

        }
};