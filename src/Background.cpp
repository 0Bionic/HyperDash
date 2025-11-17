#include "background.hpp"

Background::Background(const std::string &texturePath, float speed)
    : scrollSpeed(speed)
{
    if (!texture.loadFromFile(texturePath))
    {
        // Handle error - could throw exception or log
    }

    sprite1.setTexture(texture);
    sprite2.setTexture(texture);

    sprite1.setPosition(0, 0);
    sprite2.setPosition(sprite1.getGlobalBounds().width, 0);
}

void Background::update()
{
    sprite1.move(-scrollSpeed, 0);
    sprite2.move(-scrollSpeed, 0);

    if (sprite1.getPosition().x + sprite1.getGlobalBounds().width < 0)
    {
        sprite1.setPosition(sprite2.getPosition().x + sprite2.getGlobalBounds().width, 0);
    }

    if (sprite2.getPosition().x + sprite2.getGlobalBounds().width < 0)
    {
        sprite2.setPosition(sprite1.getPosition().x + sprite1.getGlobalBounds().width, 0);
    }
}

void Background::render(sf::RenderWindow *window)
{
    window->draw(sprite1);
    window->draw(sprite2);
}

void Background::setSpeed(float speed)
{
    scrollSpeed = speed;
}