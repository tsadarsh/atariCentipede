#include "gameObject.cpp"

class mushroomGameObj : public gameObject
{
    public:
        int health = 2;

        mushroomGameObj(std::string name, std::string textureFilePath, float posX, float posY) : gameObject(name, textureFilePath, posX, posY) {}

        void damage(int pts)
        {
            health--;
            switch (health)
            {
            case 2:
                updateTexture("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/Mushroom0.png");
                break;
            
            case 1:
                updateTexture("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/Mushroom1.png");
                break;
            
            case 0:
                setPosition(-10, -10); // mushroom positioned out of window
                break;
            
            default:
                break;
            }
        }
};


// class mushroomGameObj : public gameObject
// {
//     public:
//         int population;
//         gameObject* instances_arr;
//         sf::Texture tex;

//         mushroomGameObj(std::string name, std::string textureFilePath) : gameObject(name, textureFilePath) {
            
//         }

//         void populateRandomly(int population, sf::IntRect area)
//         {
//             this->population = population;
//             gameObject instances[population];
//             for (int i=0; i < this->population; i++)
//             {
//                 instances[i].updateTexture("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/Mushroom0.png");
//                 instances[i].setPosition(rand(), rand());   
//             }
//             instances_arr = new gameObject[population];
//             std::copy(&instances, &instances+population, &instances_arr, &instances_arr+population);
//         }

// };