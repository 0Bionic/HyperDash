#include "hyperdash.hpp"
#include <cstdlib>
#include <iostream>

PowerUp::PowerUp()
{
    isActive = false;
    duration = 5.0f;
    scrollSpeed = 2.0f;

    if (!texture.loadFromFile("sprites/PowerUp.png"))
    {
        std::cout << "Failed to load power-up sprite!" << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setScale(0.1f, 0.1f);

    float randomY = 300 + static_cast<float>(rand() % 100);
    float randomX = 1280 + static_cast<float>(rand() % 200);

    sprite.setPosition(randomX, randomY);
}

void PowerUp::spawn()
{
    float randomY = 300 + static_cast<float>(rand() % 100);
    float randomX = 1280 + static_cast<float>(rand() % 200);

    sprite.setPosition(randomX, randomY);
    isActive = false;
}

void PowerUp::activate()
{
    isActive = true;
}

void PowerUp::update()
{
    sprite.move(-scrollSpeed, 0.0f);
}

void PowerUp::render(sf::RenderWindow *window)
{
    if (!isActive)
    {
        window->draw(sprite);
    }
}

sf::FloatRect PowerUp::getHitbox() const
{
    sf::FloatRect bounds = sprite.getGlobalBounds();

    // Shrink hitbox slightly for better collision feel
    float shrinkFactor = 0.3f;
    bounds.left += bounds.width * shrinkFactor / 2.f;
    bounds.top += bounds.height * shrinkFactor / 2.f;
    bounds.width *= (1.f - shrinkFactor);
    bounds.height *= (1.f - shrinkFactor);

    return bounds;
}