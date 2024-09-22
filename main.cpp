#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <time.h>
#include <algorithm>
#include <boost/random.hpp>

const int WINDOW_WIDTH = 1036;
const int WINDOW_HEIGHT = 569;

void beginGameSequence(sf::RenderWindow* window, sf::Event* event)
{
    window->clear(sf::Color::White);

    // load starship
    sf::Texture starship_tex;
    if(!starship_tex.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/StarShip.png"))
    {
        std::cout << "Head unable to starship" << std::endl; 
    }
    sf::Sprite starship;
    starship.setTexture(starship_tex);

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
    int c = 0;
    while (1)
    {
        window->clear(bg);
        
        // move spider randomly
        
        // switch (next_spider_move)
        // {
        // case 0:
        //     spider.move(0.1, 0);
        //     break;
        // case 1:
        //     spider.move(-0.1, 0);
        // case 2:
        //     spider.move(0, 0.0);
        // case 3:
        //     spider.move(0, -0.0);
        
        // default:
        //     break;
        // }

        int move = four_steps(rng);
        std::cout << move << std::endl;
        spider.move(0.1*four_steps(rng), 0.1*four_steps(rng));

        spider.setPosition(std::max(std::min((float)WINDOW_WIDTH-48, spider.getPosition().x), 0.f), std::max(std::min((float)WINDOW_HEIGHT-29, spider.getPosition().y), 330.f));

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            std::cout << "Right key pressed" << std::endl;
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