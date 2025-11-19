#include "hyperdash.hpp"
#include <cstdlib>

Coin::Coin()
{
    value = 10;

    if (!texture.loadFromFile("sprites/coinSprite.png"))
    {
    }

    sprite.setTexture(texture);

    // Random Y position between jump height (200) and ground (400)
    float randomY = 200 + static_cast<float>(rand() % 201);

    // Spawn ahead of screen
    float randomX = 1280 + static_cast<float>(rand() % 200);

    sprite.setPosition(randomX, randomY);
}

void Coin::update()
{
    sprite.move(-5.0f, 0.0f);
}

void Coin::render(sf::RenderWindow *window)
{
    window->draw(sprite);
}
