#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <time.h>
#include <algorithm>
#include <boost/random.hpp>
#include <any>
#include <list>
#include <cmath>
#include <sstream>
#include <iterator>
#include "helper_func.cpp"
#include "gameObject.cpp"

const int WINDOW_WIDTH = 1036;
const int WINDOW_HEIGHT = 569;


class spiderGameObj : public gameObject
{
    public:
        float targetX;
        float targetY;
        float speed;
        const int NEW_TARGET_POS_DELTA = 400;
    
        spiderGameObj(std::string name) : gameObject(name) {}
        spiderGameObj(std::string name, std::string textureFilePath) : gameObject(name, textureFilePath) {}
        spiderGameObj(std::string name, std::string textureFilePath, float posX, float posY) : gameObject(name, textureFilePath, posX, posY) {}
};

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


void beginGameSequence(sf::RenderWindow* window, sf::Event* event)
{
    window->setKeyRepeatEnabled(false);
    window->clear(sf::Color::White);

    gameObject starship("startship", "/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/StarShip.png", WINDOW_WIDTH/2, WINDOW_HEIGHT-30);
    starship.updateParam("health", (int16_t)3);

    spiderGameObj spider("spider", "/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/spider.png");
    spider.setPosition(900, 400);
    spider.targetX = rand() % (WINDOW_WIDTH - 20 + 1);
    spider.targetY = (rand() % (WINDOW_HEIGHT / 2 - 20 + 1)) + WINDOW_HEIGHT / 2;
    spider.speed = 0.1;
   
    sf::Color bg(10,24,26);
    boost::random::mt19937 rng;
    boost::random::uniform_int_distribution<> four_steps(-10, 10);
    boost::random::uniform_int_distribution<> mushroom_pos_x(0, WINDOW_WIDTH);
    boost::random::uniform_int_distribution<> mushroom_pos_y(0, WINDOW_HEIGHT);

    // load mushroom
    std::vector<mushroomGameObj> mushrooms;
    for(int i=0; i<30; i++)
    {
        mushroomGameObj mushroom("mushroom"+std::to_string(i), "/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/Mushroom0.png", mushroom_pos_x(rng), mushroom_pos_y(rng));
        mushrooms.push_back(mushroom);
    }
    for(int i=0; i<30; i++)
    {
        mushrooms[i].updateTexture("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/Mushroom0.png");
    }

    // load laser
    gameObject laser("laser", "/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/laser.png", sf::IntRect(10, 10, 3, 20));
    std::vector <gameObject> lasers;

    centipedeGameObject centipede;
    centipede.spawnNew();

    sf::Clock clock;


    int loop_counter = 0;
    while (1)
    {
        float deltaTime = clock.restart().asSeconds();
        loop_counter++;
        window->clear(bg);

        if (loop_counter == spider.NEW_TARGET_POS_DELTA)
        {
            loop_counter = 0;
            spider.targetX = rand() % (WINDOW_WIDTH - 20 + 1);
            spider.targetY = (rand() % (WINDOW_HEIGHT/2 - 20 - WINDOW_HEIGHT + 1)) + WINDOW_HEIGHT/2;
        }
        
        if (spider.getPosX() < spider.targetX)
        {
            spider.move(spider.speed, 0);
        }
        if (spider.getPosX() > spider.targetX)
        {
            spider.move(-spider.speed, 0);
        }
        if (spider.getPosY() < spider.targetY)
        {
            spider.move(0, spider.speed);
        }
        if (spider.getPosY() > spider.targetY)
        {
            spider.move(0, -spider.speed);
        }

        sf::Event event;
        while (window->pollEvent(event))
        {
            // check the type of the event...
            switch (event.type)
            {
                // window closed
                case sf::Event::Closed:
                    window->close();
                    break;

                // key pressed
                case sf::Event::KeyPressed:
                    if (event.type == sf::Event::KeyPressed)
                    {
                        if (event.key.code == sf::Keyboard::Space)
                        {
                            gameObject laser("laser", "/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/laser.png", sf::IntRect(10, 10, 3, 20));
                            lasers.push_back(laser);
                            lasers.back().setPosition(starship.getPosX(), starship.getPosY());  
                        }
                    }
                    break;

                // we don't process other types of events
                default:
                    break;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            //std::cout << "Right key pressed" << std::endl;
            // left key is pressed: move our character
            starship.move(0.1f, 0.f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            starship.move(-0.1f, 0.f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            starship.move(0.f, -0.1f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            starship.move(0.f, 0.1f);
        }

        // Control the lead particle (the first one)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            centipede.move(0, -200*deltaTime, deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            centipede.move(0, 200*deltaTime, deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            centipede.move(-200*deltaTime, 0, deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            centipede.move(200*deltaTime, 0, deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            centipede.spawnNew();
        }


        if(starship.collisonBox.intersects(spider.collisonBox))
        {
            starship.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT-30);
            std::cout << "You are dead!!" << std::endl;
            int16_t currentHealth = std::any_cast<std::int16_t>(starship.getParam("health"));
            starship.updateParam("health", (int16_t)(currentHealth - 1));
        }
        
        int co = 1;
        for(auto i_laser=lasers.begin(); i_laser != lasers.end(); i_laser++)
        for(int i_laser = 0; i_laser < lasers.size(); i_laser++)
        {
            float l_x = lasers[i_laser].getPosX();
            float l_y = lasers[i_laser].getPosY();

            if(l_y > 0)
            {
                for (int i_mushroom = 0; i_mushroom < mushrooms.size() ; i_mushroom++)
                {
                    if(mushrooms[i_mushroom].collisonBox.contains(l_x, l_y) & mushrooms[i_mushroom].health > 0)
                    {
                        mushrooms[i_mushroom].damage(1);
                        lasers[i_laser].setPosition(-10, -10);
                    }
                }

                if (spider.collisonBox.contains(l_x, l_y))
                {
                    std::cout << "Spider Killed!" << std::endl;
                    spider.setPosition(900, 400);
                }

                for (int i_centipedeFamily = 0; i_centipedeFamily < centipede.family.size(); i_centipedeFamily++)
                {
                    for (int i_body = 0; i_body < centipede.family[i_centipedeFamily].size(); i_body++)
                    {
                        if (centipede.family[i_centipedeFamily][i_body].collisonBox.contains(l_x, l_y))
                        {
                            lasers[i_laser].setPosition(-10, -10);
                            centipede.spawnNew();
                            std::cout << "Laser hit centipede!" << std::endl;
                        }
                    }
                }

                lasers[i_laser].move(0.f, -0.1f);
                window->draw(lasers[i_laser].sprite);
            }
        }

        for(int i=0; i<30; i++)
        {
            window->draw(mushrooms[i].sprite);
        }

        if (std::any_cast<std::int16_t>(starship.getParam("health")) > 0)
        {
            window->draw(starship.sprite);
        }

        std::cout << centipede.family[0][0].sprite.getPosition().x << ", " << centipede.family[0][0].collisonBox.left << std::endl;
        for (int i_family = 0; i_family < centipede.family.size(); i_family++)
        {
            for (int i=0; i<11; i++)
            {
                window->draw(centipede.family[i_family][i].sprite);
            }
        }
        window->draw(spider.sprite);
        window->display();
    }
}

int main()
{   
    srand(time(NULL));
    // create the window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "My window");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        sf::Texture texture;
        if (!texture.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/startup_screen_background.png"))
        {
            std::cout << "No texture found!";
        }

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Enter)
                {
                    std::cout << "Enter key pressed" << std::endl;
                    beginGameSequence(&window, &event);
                }
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        sf::Sprite sprite;
        sprite.setTexture(texture);
        window.draw(sprite);

        // end the current frame
        window.display();
    }

    return 0;
}