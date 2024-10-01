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
#include "gameObject.cpp"
#include "starshipGameObj.cpp"
#include "spiderGameObj.cpp"
#include "mushroomGameObj.cpp"
#include "laserGameObj.cpp"
#include "centipedeGameObj.cpp"

const int WINDOW_WIDTH = 1036;
const int WINDOW_HEIGHT = 569;
sf::Texture starshipTexture, spiderTexture, mushroomHealth2Texture, mushroomHealth1Texture, laserTexture, centipedeHeadTexture, centipedeBodyTexture;


void spawnCentipedeAndAddToFamily(std::vector<centipedeGameObject>* familyContainer, int lengthOfCentipede, float spawnX, float spawnY)
{
    if (lengthOfCentipede > 0) 
    {
        centipedeGameObject centipede("centipede" + (*familyContainer).size(), &centipedeHeadTexture, &centipedeBodyTexture);
        centipede.spawn(lengthOfCentipede, spawnX, spawnY);
        (*familyContainer).push_back(centipede);
    }
    else
    {
        std::cout << "Invalid length of centipede requested: " << lengthOfCentipede << std::endl;
    }
}

void spawnLaserAndAddToFamily (std::vector<laserGameObj>* familyContainer, float spawnX, float spawnY)
{
    laserGameObj laser("laser" + (*familyContainer).size(), &laserTexture);
    laser.m_sprite.setPosition(spawnX, spawnY);
    (*familyContainer).push_back(laser);
}

void beginGameSequence(sf::RenderWindow* window, sf::Event* event)
{
    window->setKeyRepeatEnabled(false);
    window->clear(sf::Color::White);

    starshipGameObj starship("startship", &starshipTexture);

    spiderGameObj spider("spider", &spiderTexture);
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
        mushroomGameObj mushroom("mushroom"+std::to_string(i), &mushroomHealth2Texture, &mushroomHealth1Texture);
        mushroom.setPosition(mushroom_pos_x(rng), mushroom_pos_y(rng));
        mushrooms.push_back(mushroom);
    }

    // load laser
    std::vector <laserGameObj> lasers;

    // load centipede
    std::vector<centipedeGameObject> centipedeFamily;
    spawnCentipedeAndAddToFamily(&centipedeFamily, 12, 400, 300);

    sf::Clock clock;


    int loop_counter = 0;

    float c_moveDir_x = -0.1;
    float c_moveDir_y = 0;
    float c_moveSpeed = 1;

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
        
        for (int i_family = 0; i_family < centipedeFamily.size(); i_family++)
        {
            centipedeFamily[i_family].move(centipedeFamily[i_family].speedX, 0, deltaTime);
        }

        if (spider.m_sprite.getPosition().x < spider.targetX)
        {
            spider.move(spider.speed, 0);
        }
        if (spider.m_sprite.getPosition().x > spider.targetX)
        {
            spider.move(-spider.speed, 0);
        }
        if (spider.m_sprite.getPosition().y < spider.targetY)
        {
            spider.move(0, spider.speed);
        }
        if (spider.m_sprite.getPosition().y > spider.targetY)
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
                            spawnLaserAndAddToFamily(&lasers, starship.m_sprite.getPosition().x, starship.m_sprite.getPosition().y);
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
        float centipede_teleop_dir_x = 0;
        float centipede_teleop_dir_y = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            centipede_teleop_dir_x = 0;
            centipede_teleop_dir_y = -200*deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            centipede_teleop_dir_x = 0;
            centipede_teleop_dir_y = 200*deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            centipede_teleop_dir_x = -200*deltaTime;
            centipede_teleop_dir_y = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            centipede_teleop_dir_x = 200*deltaTime;
            centipede_teleop_dir_y = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            spawnCentipedeAndAddToFamily(&centipedeFamily, 12, 400, 300);
        }
        for (int i_centipede = 0; i_centipede < centipedeFamily.size(); i_centipede++)
        {
            centipedeFamily[i_centipede].move(centipede_teleop_dir_x, centipede_teleop_dir_y, deltaTime);
        }


        if(starship.m_collisonBox.intersects(spider.m_collisonBox))
        {
            starship.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT-30);
            std::cout << "You are dead!!" << std::endl;
            starship.m_health--;
        }
        
        for(int i_mushroom = 0; i_mushroom < mushrooms.size(); i_mushroom++)
        {
            if (mushrooms[i_mushroom].m_collisonBox.intersects(spider.m_collisonBox))
            {
                mushrooms[i_mushroom].damage(1);
            }
        }

        for(int i_laser = 0; i_laser < lasers.size(); i_laser++)
        {
            if(lasers[i_laser].m_sprite.getPosition().y > 0)
            {
                for (int i_mushroom = 0; i_mushroom < mushrooms.size() ; i_mushroom++)
                {
                    if(mushrooms[i_mushroom].m_collisonBox.contains
                    (
                        lasers[i_laser].m_sprite.getPosition().x, \
                        lasers[i_laser].m_sprite.getPosition().y)  \
                        & mushrooms[i_mushroom].health > 0
                    )
                    {
                        mushrooms[i_mushroom].damage(1);
                        lasers[i_laser].setPosition(0, -100);
                    }
                }

                if (spider.m_collisonBox.contains(lasers[i_laser].m_sprite.getPosition().x, lasers[i_laser].m_sprite.getPosition().y))
                {
                    std::cout << "Spider Killed!" << std::endl;
                    spider.setPosition(900, 400);
                }

                for (int i_centipedeFamily = 0; i_centipedeFamily < centipedeFamily.size(); i_centipedeFamily++)
                {
                    bool b_collidedAlready = false;
                    for (int i_body = 0; i_body < centipedeFamily[i_centipedeFamily].centipede.size(); i_body++)
                    {
                        bool bb_collidedAlready = false;
                        if (centipedeFamily[i_centipedeFamily].centipede[i_body].m_collisonBox.contains \
                                (
                                    lasers[i_laser].m_sprite.getPosition().x, \
                                    lasers[i_laser].m_sprite.getPosition().y \
                                )
                            )
                        {
                            lasers[i_laser].setPosition(0, -100);
                            spawnCentipedeAndAddToFamily(&centipedeFamily, centipedeFamily[i_centipedeFamily].centipede.size() - 1, \
                                centipedeFamily[i_centipedeFamily].centipede[i_body].m_sprite.getPosition().x, 
                                centipedeFamily[i_centipedeFamily].centipede[i_body].m_sprite.getPosition().y
                            );
                            centipedeFamily[i_centipedeFamily].centipede.resize(i_body);
                            centipedeFamily.back().speedX = (-1) * centipedeFamily[i_centipedeFamily].speedX;
                            std::cout << i_centipedeFamily << " :Laser hit centipede!" << std::endl;
                            bb_collidedAlready = true;
                            break;
                        }
                        if (bb_collidedAlready)
                        {
                            b_collidedAlready = true;
                            break;
                        }
                    }
                    if (b_collidedAlready)
                        break;
                }
                lasers[i_laser].move(0.f, -0.1f);
                window->draw(lasers[i_laser].m_sprite);
            }
        }

        for (int i_centipede = 0; i_centipede < centipedeFamily.size(); i_centipede++)
        {
            if ((centipedeFamily[i_centipede].centipede[0].m_sprite.getPosition().x > WINDOW_WIDTH) \
                 | (centipedeFamily[i_centipede].centipede[0].m_sprite.getPosition().x < 0))
            {
                centipedeFamily[i_centipede].speedX *= -1;
                centipedeFamily[i_centipede].move(0, centipedeFamily[i_centipede].moveY, deltaTime);
            }
            if ((centipedeFamily[i_centipede].centipede[0].m_sprite.getPosition().y > WINDOW_HEIGHT) \
                | (centipedeFamily[i_centipede].centipede[0].m_sprite.getPosition().y < 0))
            {
                centipedeFamily[i_centipede].moveY *= -1;
                centipedeFamily[i_centipede].move(0, centipedeFamily[i_centipede].moveY, deltaTime);
            }
            
            for (int i_mushroom = 0; i_mushroom < mushrooms.size(); i_mushroom++)
            {
                if (mushrooms[i_mushroom].health > 0 & centipedeFamily[i_centipede].centipede[0].m_collisonBox.intersects(mushrooms[i_mushroom].m_collisonBox)) 
                {
                    centipedeFamily[i_centipede].speedX *= -1;
                    //centipedeFamily[i_centipede].move(0, centipedeFamily[i_centipede].moveY, deltaTime);
                }
            }
        }

        for(int i=0; i<30; i++)
        {
            window->draw(mushrooms[i].m_sprite);
        }

        if (starship.m_health > 0)
        {
            window->draw(starship.m_sprite);
        }

        // std::cout << centipede.family[0][0].sprite.getPosition().x << ", " << centipede.family[0][0].collisonBox.left << std::endl;
        for (int i_family = 0; i_family < centipedeFamily.size(); i_family++)
        {
            for (int i_body=0; i_body < centipedeFamily[i_family].centipede.size(); i_body++)
            {
                window->draw(centipedeFamily[i_family].centipede[i_body].m_sprite);
            }
        }
        window->draw(spider.m_sprite);
        window->display();
    }
}

int main()
{   
    srand(time(NULL));
    // create the window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "My window");

    if(!starshipTexture.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/StarShip.png"))
    {
        std::cout << "Error loading starship texture" << std::endl;
    }

    if(!spiderTexture.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/spider.png"))
    {
        std::cout << "Error loading spider texture" << std::endl;
    }

    if(!mushroomHealth2Texture.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/Mushroom0.png"))
    {
        std::cout << "Error loading musgroomHealth2 texture" << std::endl;
    }

    if(!mushroomHealth1Texture.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/Mushroom1.png"))
    {
        std::cout << "Error loading mushroomHealth1 texture" << std::endl;
    }
        
    if(!laserTexture.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/laser.png",sf::IntRect(10, 10, 3, 20)))
    {
        std::cout << "Error loading laser texture" << std::endl;
    }

    if(!centipedeHeadTexture.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/CentipedeHead.png"))
    {
        std::cout << "Error loading centipedeHeadTexture texture" << std::endl;
    }

    if(!centipedeBodyTexture.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/CentipedeBody.png"))
    {
        std::cout << "Error loading centipedeBodyTexture texture" << std::endl;
    }

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