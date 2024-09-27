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
            this->texture = newTexture;
        }
        sf::Texture texture;
        std::string name;
        void updateCollisonBox(float padding)
        {
            this->collisonBox = sf::IntRect(this->getPosX() - padding, this->getPosY() - padding, \
                                            this->sprite.getGlobalBounds().width + (2 * padding), \
                                            this->sprite.getGlobalBounds().height + (2 * padding));
        }

        std::map<std::string, std::any> params;

    public:
        bool collisionEnabled = true;
        float collsionPadding = 10;
        sf::IntRect collisonBox;
        sf::Sprite sprite;

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
                this->updateCollisonBox(this->collsionPadding);
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
            this->sprite.setPosition(this->getPosX() + delX, this->getPosY() + delY);
        }

};


void beginGameSequence(sf::RenderWindow* window, sf::Event* event)
{
    window->setKeyRepeatEnabled(false);
    window->clear(sf::Color::White);

    gameObject starship("startship", "/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/StarShip.png", WINDOW_WIDTH/2, WINDOW_HEIGHT-30);
    starship.updateParam("health", (int16_t)3);
    // load starship
    // sf::Texture starship_tex;
    // if(!starship_tex.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/StarShip.png"))
    // {
    //     std::cout << "Head unable to starship" << std::endl; 
    // }

    // newTextureObj starship_tex = loadNewTexture("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/StarShip.png", "startship");

    // struct starship_obj
    // {
    //     sf::Sprite obj;
    //     sf::IntRect collisionBox;
    //     int health;
    // };
    // starship_obj starship;
    // starship.obj.setTexture(starship_tex.tex);
    // starship.obj.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT-30);
    // starship.collisionBox = sf::IntRect(starship.obj.getPosition().x-10, starship.obj.getPosition().y-10, 35, 35);
    //starship.health = 3;

    // load spider
    sf::Texture spider_tex;
    if(!spider_tex.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/spider.png"))
    {
        std::cout << "Head unable to spider" << std::endl; 
    }
    
    struct spider_obj
    {
        sf::Sprite obj;
        float x;
        float y;
        sf::IntRect collisionBox;
        float target_x;
        float target_y;
        int NEW_TARGET_POS_DELTA; // iterations count to pass before new target cords for spider
        float speed;
    };

    spider_obj spider;
    spider.obj = sf::Sprite(spider_tex);
    spider.x = 900.0;
    spider.y = 400.0;
    spider.obj.setPosition(spider.x, spider.y);
    spider.target_x = (rand() % (WINDOW_WIDTH - 20 + 1));
    spider.target_y = ((rand() % (WINDOW_HEIGHT/2 - 20 + 1))) + WINDOW_HEIGHT/2;
    spider.speed = 0.1;
    spider.NEW_TARGET_POS_DELTA = 400;
    spider.collisionBox = sf::IntRect(spider.obj.getPosition().x-10, spider.obj.getPosition().y-10, 35, 35);


    sf::Color bg(10,24,26);
    boost::random::mt19937 rng;
    boost::random::uniform_int_distribution<> four_steps(-10, 10);
    boost::random::uniform_int_distribution<> mushroom_pos_x(0, WINDOW_WIDTH);
    boost::random::uniform_int_distribution<> mushroom_pos_y(0, WINDOW_HEIGHT);

    // load mushroom
    sf::Texture mushroom_tex;
    if(!mushroom_tex.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/Mushroom0.png"))
    {
        std::cout << "Head unable to mushroom0" << std::endl; 
    }
    const int NUMBER_OF_MUSHROOMS = 30;
    sf::Texture MUSHROOM_HEALTH_2_TEX;
    if(!MUSHROOM_HEALTH_2_TEX.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/Mushroom0.png"))
    {
        std::cout << "Head unable to health 2 mushroom" << std::endl; 
    }

    sf::Texture MUSHROOM_HEALTH_1_TEX;
    if(!MUSHROOM_HEALTH_1_TEX.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/Mushroom1.png"))
    {
        std::cout << "Head unable to health 1 mushroom" << std::endl; 
    }

    struct mushroom_obj
    {
        sf::Sprite obj;
        int x;
        int y;
        int health;
        sf::IntRect collisionBox;
    };

    const int MAX_MUSHROOM_COUNT = 30;
    mushroom_obj mushrooms[MAX_MUSHROOM_COUNT];
    for (int i=0; i<MAX_MUSHROOM_COUNT; i++)
    {
        mushrooms[i].obj = sf::Sprite(MUSHROOM_HEALTH_2_TEX);
        mushrooms[i].x = mushroom_pos_x(rng);
        mushrooms[i].y = mushroom_pos_y(rng);
        mushrooms[i].health = 2;
        mushrooms[i].obj.setPosition(mushrooms[i].x, mushrooms[i].y);
        mushrooms[i].collisionBox = sf::IntRect(mushrooms[i].x-10, mushrooms[i].y-10, 35, 35);
    }

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
            spider.target_x = rand() % (WINDOW_WIDTH - 20 + 1);
            spider.target_y = (rand() % (WINDOW_HEIGHT/2 - 20 - WINDOW_HEIGHT + 1)) + WINDOW_HEIGHT/2;
        }
        
        if (spider.obj.getPosition().x < spider.target_x)
        {
            spider.obj.move(spider.speed, 0);
        }
        if (spider.obj.getPosition().x > spider.target_x)
        {
            spider.obj.move(-spider.speed, 0);
        }
        if (spider.obj.getPosition().y < spider.target_y)
        {
            spider.obj.move(0, spider.speed);
        }
        if (spider.obj.getPosition().y > spider.target_y)
        {
            spider.obj.move(0, -spider.speed);
        }
        spider.collisionBox = sf::IntRect(spider.obj.getPosition().x-10, spider.obj.getPosition().y-10, 35, 35);
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
        //starship.collisionBox = sf::IntRect(starship.obj.getPosition().x-10, starship.obj.getPosition().y-10, 35, 35);

        if(starship.collisonBox.intersects(spider.collisionBox))
        {
            std::cout << "You are dead!!" << std::endl;
            int16_t currentHealth = std::any_cast<std::int16_t>(starship.getParam("health"));
            starship.updateParam("health", (int16_t)(currentHealth - 1));
            starship.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT-30);
            //starship.obj.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT-30);
        }
        
        int co = 1;
        for(laser_it=lasers.begin(); laser_it!=lasers.end(); ++laser_it)
        {
            int l_x = (*laser_it).getPosition().x;
            int l_y = (*laser_it).getPosition().y;

            if(l_y > 0)
            {
                for (int i=0; i < NUMBER_OF_MUSHROOMS; i++)
                {
                    if (mushrooms[i].collisionBox.contains(l_x, l_y) and mushrooms[i].health > 0)
                    {
                        mushrooms[i].health--;
                        mushrooms[i].obj.setTexture(MUSHROOM_HEALTH_1_TEX); 
                        (*laser_it).setPosition(-10, -10); // laser positioned out of window
                    }
                    if (spider.collisionBox.contains(l_x, l_y))
                    {
                        std::cout << "Spider Killed!" << std::endl;
                        spider.obj.setPosition(sf::Vector2f(900, 400));
                    }
                }
                if(l_y < 200)
                {
                    (*laser_it).setTexture(new_laser_tex);
                }
                (*laser_it).move(0.f, -0.1f);
                window->draw(*laser_it);
            }
        }

        for(int i=0; i<30; i++)
        {
            if(mushrooms[i].health > 0)
            {
                // if(mushrooms[i].health == 1)
                // {
                //     mushrooms[i].obj.setTexture(MUSHROOM_HEALTH_1_TEX);
                // }
                window->draw(mushrooms[i].obj);

            }
        }
        if (std::any_cast<std::int16_t>(starship.getParam("health")) > 0)
        {
            window->draw(starship.sprite);
        }
        
        window->draw(spider.obj);
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