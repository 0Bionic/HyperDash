#include "hyperdash.hpp"
#include <cstdlib>
#include <iostream>

Coin::Coin()
{
    value = 20;
    scrollSpeed = 2.0f;

    if (!texture.loadFromFile("sprites/coinSprite.png"))
    {
        std::cout << "Failed to load coin sprite!" << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setScale(0.0625f, 0.0625f);

    float randomY = 300 + static_cast<float>(rand() % 81);
    float randomX = 1280 + static_cast<float>(rand() % 200);

    sprite.setPosition(randomX, randomY);
}

sf::FloatRect Coin::getHitbox() const
{
    sf::FloatRect bounds = sprite.getGlobalBounds();

    float shrinkFactor = 0.5f;
    bounds.left += bounds.width * shrinkFactor / 2.f;
    bounds.top += bounds.height * shrinkFactor / 2.f;
    bounds.width *= (1.f - shrinkFactor);
    bounds.height *= (1.f - shrinkFactor);

    return bounds;
}

void Coin::update()
{
    sprite.move(-scrollSpeed, 0.0f);
}

void Coin::render(sf::RenderWindow *window)
{
    window->draw(sprite);
}