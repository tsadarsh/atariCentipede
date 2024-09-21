#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>

void beginGameSequence(sf::RenderWindow* window, sf::Event* event)
{
    window->clear(sf::Color::White);
    sf::Texture head;
    if(!head.loadFromFile("/home/ada/6122/Beginning-Cpp-Game-Programming-Second-Edition/Lab1/sprites/CentipedeHead.png"))
    {
        std::cout << "Head unable to load" << std::endl; 
    }
    sf::Sprite head_sprite;
    head_sprite.setTexture(head);
    //head_sprite.setTextureRect(sf::IntRect(-100, -100, 100, 100));

    while (1)
    {
        window->clear(sf::Color::White);
        std::cout << head_sprite.getPosition().x << std::endl;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            std::cout << "Right key pressed" << std::endl;
            // left key is pressed: move our character
            head_sprite.move(0.1f, 0.f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            head_sprite.move(-0.1f, 0.f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            head_sprite.move(0.f, -0.1f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            head_sprite.move(0.f, 0.1f);
        }
        
        window->draw(head_sprite);
        window->display();
    }
}

int main()
{   
    // create the window
    sf::RenderWindow window(sf::VideoMode(1036, 569), "My window");

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