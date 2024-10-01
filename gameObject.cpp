#ifndef _GAME_OBJ
#define _GAME_OBJ

#include <iostream>
#include <SFML/Graphics.hpp>
#include <any>

extern sf::Sprite gameObjSprite;

class gameObject
{
    public:
        std::string m_name;
        sf::Sprite m_sprite;
        sf::IntRect m_collisonBox;

        bool m_collisionEnabled = true;
        float m_collisionPadding = 10;

        gameObject () {}

        gameObject (std::string name)
        {
            m_name = name;
        }

        gameObject (std::string name, sf::Sprite sprite)
        {
            m_name = name;
            m_sprite = sprite;
        }

        gameObject (std::string name, sf::Sprite sprite, float posX, float posY)
        {
            m_name = name;
            m_sprite = sprite;
            setPosition(posX, posY);
        }

        void setPosition(float posX, float posY)
        {
            m_sprite.setPosition(posX, posY);
            if (m_collisionEnabled)
            {
                updateCollisonBox();
            }
        }
        
        void move(float delX, float delY)
        {
            // moves sprite along with collision box;
            this->setPosition(m_sprite.getPosition().x + delX, m_sprite.getPosition().y + delY);
        }

        void updateCollisonBox()
        {
            m_collisonBox = sf::IntRect(m_sprite.getPosition().x - m_collisionPadding, m_sprite.getPosition().y - m_collisionPadding, \
                                            m_sprite.getTexture()->getSize().x + (2 * m_collisionPadding), \
                                            m_sprite.getTexture()->getSize().y + (2 * m_collisionPadding));
        }

};

#endif