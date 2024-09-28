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

const int WINDOW_WIDTH = 1036;
const int WINDOW_HEIGHT = 569;


class gameObject
{
    private:
        void loadTexture (std::string textureFilePath)
        {
            sf::Texture newTexture;
            if(!newTexture.loadFromFile(textureFilePath))
            {
                std::cout << "Error loading texture from: " << textureFilePath << std::endl;
            }
            this->textureFilePath = textureFilePath;
            this->texture = newTexture;
        }
        std::string name;
        void updateCollisonBox()
        {
            this->collisonBox = sf::IntRect(this->getPosX() - this->collsionPadding, this->getPosY() - this->collsionPadding, \
                                            this->sprite.getTexture()->getSize().x + (2 * this->collsionPadding), \
                                            this->sprite.getTexture()->getSize().y + (2 * this->collsionPadding));
        }

        std::map<std::string, std::any> params;

    public:
        sf::Texture texture;
        std::string textureFilePath;
        bool collisionEnabled = true;
        float collsionPadding = 10;
        sf::IntRect collisonBox;
        sf::Sprite sprite;

        gameObject () {}
        gameObject (std::string name) 
        {
            this->name = name;
        }
        gameObject (std::string name, std::string textureFilePath)
        {
            this->name = name;
            this->loadTexture(textureFilePath);
            this->updateTexture(this->texture);
        }
        gameObject (std::string name, sf::Texture textureFile)
        {
            this->name = name;
            this->updateTexture(textureFile);
        }
        gameObject (std::string name, std::string textureFilePath, float posX, float posY)
        {
            this->name = name;
            this->loadTexture(textureFilePath);
            this->updateTexture(this->texture);
            this->setPosition(posX, posY);
        }
        gameObject (std::string name, sf::Texture textureFile, float posX, float posY)
        {
            this->name = name;
            this->updateTexture(textureFile);
            this->sprite.setPosition(posX, posY);
        }
        void setPosition(float posX, float posY)
        {
            this->sprite.setPosition(posX, posY);
            if (collisionEnabled)
            {
                this->updateCollisonBox();
            }
        }
        void updateTexture (sf::Texture newTexture)
        {
            this->texture = newTexture;
            this->sprite.setTexture(this->texture);
        }
        void updateTexture (std::string textureFilePath)
        {
            this->loadTexture(textureFilePath);
            this->sprite.setTexture(this->texture);
        }
        std::string getName()
        {
            return this->name;
        }
        float getPosX()
        {
            return this->sprite.getPosition().x;
        }
        float getPosY()
        {
            return this->sprite.getPosition().y;
        }
        void updateParam(std::string key, std::any value)
        {
            this->params[key] = value;
        }
        std::any getParam(std::string key)
        {
            return this->params[key];
        }
        void move(float delX, float delY)
        {
            this->setPosition(this->getPosX() + delX, this->getPosY() + delY);
        }

};


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
        int population;
        std::vector<gameObject> instances;
        sf::Texture tex;

        mushroomGameObj(std::string name, std::string textureFilePath) : gameObject(name, textureFilePath) {
            this->tex.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/Mushroom0.png");
        }

        void populateRandomly(int population, sf::IntRect area)
        {
            this->population = population;
            for (int i=0; i < this->population; i++)
            {
                std::cout << this->textureFilePath << std::endl;
                gameObject mushroom("mushroom"+std::to_string(i), tex, (rand() % area.width) + area.left, (rand() %  area.height) + area.top);
                this->instances.push_back(mushroom);
            }
        }

};


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
    mushroomGameObj mushrooms("mushrooms", "/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/Mushroom0.png");
    mushrooms.populateRandomly(30, sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_WIDTH));


    // sf::Texture mushroom_tex;
    // if(!mushroom_tex.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/Mushroom0.png"))
    // {
    //     std::cout << "Head unable to mushroom0" << std::endl; 
    // }
    // const int NUMBER_OF_MUSHROOMS = 30;
    // sf::Texture MUSHROOM_HEALTH_2_TEX;
    // if(!MUSHROOM_HEALTH_2_TEX.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/Mushroom0.png"))
    // {
    //     std::cout << "Head unable to health 2 mushroom" << std::endl; 
    // }

    // sf::Texture MUSHROOM_HEALTH_1_TEX;
    // if(!MUSHROOM_HEALTH_1_TEX.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/Mushroom1.png"))
    // {
    //     std::cout << "Head unable to health 1 mushroom" << std::endl; 
    // }

    // struct mushroom_obj
    // {
    //     sf::Sprite obj;
    //     int x;
    //     int y;
    //     int health;
    //     sf::IntRect collisionBox;
    // };

    // const int MAX_MUSHROOM_COUNT = 30;
    // mushroom_obj mushrooms[MAX_MUSHROOM_COUNT];
    // for (int i=0; i<MAX_MUSHROOM_COUNT; i++)
    // {
    //     mushrooms[i].obj = sf::Sprite(MUSHROOM_HEALTH_2_TEX);
    //     mushrooms[i].x = mushroom_pos_x(rng);
    //     mushrooms[i].y = mushroom_pos_y(rng);
    //     mushrooms[i].health = 2;
    //     mushrooms[i].obj.setPosition(mushrooms[i].x, mushrooms[i].y);
    //     mushrooms[i].collisionBox = sf::IntRect(mushrooms[i].x-10, mushrooms[i].y-10, 35, 35);
    // }

    int move = four_steps(rng);

    int c = 0;

    // load laser
    sf::Texture laser_tex;
    if(!laser_tex.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/laser.png", sf::IntRect(10, 10, 3, 20)))
    {
        std::cout << "Head unable to laser" << std::endl;
    }
    std::list <sf::Sprite> lasers;
    std::list<sf::Sprite>::iterator laser_it;

    sf::Texture new_laser_tex;
    if(!new_laser_tex.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/violet-neon.jpg", sf::IntRect(10, 10, 3, 20)))
    {
        std::cout << "No new laser" << std::endl;
    }


    int loop_counter = 0;
    while (1)
    {
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
                            lasers.push_back(sf::Sprite(laser_tex));
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

        if(starship.collisonBox.intersects(spider.collisonBox))
        {
            starship.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT-30);
            std::cout << "You are dead!!" << std::endl;
            int16_t currentHealth = std::any_cast<std::int16_t>(starship.getParam("health"));
            starship.updateParam("health", (int16_t)(currentHealth - 1));
        }
        
        int co = 1;
        // for(laser_it=lasers.begin(); laser_it!=lasers.end(); ++laser_it)
        // {
        //     int l_x = (*laser_it).getPosition().x;
        //     int l_y = (*laser_it).getPosition().y;

        //     if(l_y > 0)
        //     {
        //         for (int i=0; i < NUMBER_OF_MUSHROOMS; i++)
        //         {
        //             if (mushrooms[i].collisionBox.contains(l_x, l_y) and mushrooms[i].health > 0)
        //             {
        //                 mushrooms[i].health--;
        //                 mushrooms[i].obj.setTexture(MUSHROOM_HEALTH_1_TEX); 
        //                 (*laser_it).setPosition(-10, -10); // laser positioned out of window
        //             }
        //             if (spider.collisonBox.contains(l_x, l_y))
        //             {
        //                 std::cout << "Spider Killed!" << std::endl;
        //                 spider.setPosition(900, 400);
        //             }
        //         }
        //         if(l_y < 200)
        //         {
        //             (*laser_it).setTexture(new_laser_tex);
        //         }
        //         (*laser_it).move(0.f, -0.1f);
        //         window->draw(*laser_it);
        //     }
        // }

        // for(int i=0; i<30; i++)
        // {
        //     if(mushrooms[i].health > 0)
        //     {
        //         // if(mushrooms[i].health == 1)
        //         // {
        //         //     mushrooms[i].obj.setTexture(MUSHROOM_HEALTH_1_TEX);
        //         // }
        //         window->draw(mushrooms[i].obj);

        //     }
        // }
        for (gameObject i : mushrooms.instances)
        {
            std::cout << "path:" << i.textureFilePath << std::endl;
            window->draw(i.sprite);
        }
        if (std::any_cast<std::int16_t>(starship.getParam("health")) > 0)
        {
            window->draw(starship.sprite);
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