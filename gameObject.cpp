#ifndef _GAME_OBJ
#define _GAME_OBJ

#include <iostream>
#include <SFML/Graphics.hpp>
#include <any>

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
        void loadTexture (std::string textureFilePath, sf::IntRect area)
        {
            sf::Texture newTexture;
            if(!newTexture.loadFromFile(textureFilePath, area))
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
        gameObject (std::string name, std::string textureFilePath, sf::IntRect area)
        {
            this->name = name;
            this->loadTexture(textureFilePath, area);
            this->updateTexture(this->texture);
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

#endif