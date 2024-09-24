#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <time.h>
#include <algorithm>
#include <boost/random.hpp>
#include <list>

const int WINDOW_WIDTH = 1036;
const int WINDOW_HEIGHT = 569;

void beginGameSequence(sf::RenderWindow* window, sf::Event* event)
{
    window->setKeyRepeatEnabled(false);
    window->clear(sf::Color::White);

    // load starship
    sf::Texture starship_tex;
    if(!starship_tex.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/StarShip.png"))
    {
        std::cout << "Head unable to starship" << std::endl; 
    }
    sf::Sprite starship;
    starship.setTexture(starship_tex);
    starship.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT-30);

    // load spider
    sf::Texture spider_tex;
    if(!spider_tex.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/spider.png"))
    {
        std::cout << "Head unable to spider" << std::endl; 
    }
    sf::Sprite spider;
    spider.setTexture(spider_tex);
    spider.setPosition(900, 400);

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
    sf::Sprite mushrooms[30];
    for(int i=0; i<30; i++)
    {
        mushrooms[i] = sf::Sprite(mushroom_tex);
        mushrooms[i].setPosition(mushroom_pos_x(rng), mushroom_pos_y(rng));
        window->draw(mushrooms[i]);
    }
    starship.setTexture(starship_tex);
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

    while (1)
    {
        window->clear(bg);
    
        spider.move(0.1*four_steps(rng), 0.1*four_steps(rng));

        spider.setPosition(std::max(std::min((float)WINDOW_WIDTH-48, spider.getPosition().x), 0.f), std::max(std::min((float)WINDOW_HEIGHT-29, spider.getPosition().y), 330.f));

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
                            lasers.back().setPosition(starship.getPosition());   
                        }

                        else if (event.key.code == sf::Keyboard::Right)
                        {
                            //std::cout << "Right key pressed" << std::endl;
                            // left key is pressed: move our character
                            starship.move(0.1f, 0.f);
                        }
                        else if (event.key.code == sf::Keyboard::Left)
                        {
                            starship.move(-0.1f, 0.f);
                        }
                        else if (event.key.code == sf::Keyboard::Up)
                        {
                            starship.move(0.f, -0.1f);
                        }
                        else if (event.key.code == sf::Keyboard::Down)
                        {
                            starship.move(0.f, 0.1f);
                        }
                    }
                    break;

                // we don't process other types of events
                default:
                    break;
            }
        }
        // if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        // {
        //     lasers.push_back(sf::Sprite(laser_tex));
        //     lasers.back().setPosition(starship.getPosition());
        // }

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
        
        int co = 1;
        for(laser_it=lasers.begin(); laser_it!=lasers.end(); ++laser_it)
        {
            if((*laser_it).getPosition().y > 0)
            {
                (*laser_it).move(0.f, -0.1f);
                window->draw(*laser_it);
            }
        }

        for(int i=0; i<30; i++)
        {
            window->draw(mushrooms[i]);
        }
        window->draw(starship);
        window->draw(spider);
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