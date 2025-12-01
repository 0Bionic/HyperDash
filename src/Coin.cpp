#include "hyperdash.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>

Coin::Coin()
{
    srand(static_cast<unsigned>(time(0)));
    value = 10;

    if (!texture.loadFromFile("sprites/coinSprite.png"))
    {
        std::cout << "fail";
    }

    sprite.setTexture(texture);
    sprite.setScale(0.1f, 0.1f);

    float randomY = 400 + static_cast<float>(rand() % 100);

    // Spawn ahead of screen
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
    sprite.move(-5.0f, 0.0f);
}

void Coin::render(sf::RenderWindow *window)
{
    window->draw(sprite);
}
