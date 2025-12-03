#include "hyperdash.hpp"
#include <cstdlib>
#include <iostream>

PowerUp::PowerUp()
{
    isActive = false;
    duration = 5.0f;
    scrollSpeed = 2.0f;

    // Random Type: 0=Invincible, 1=RapidFire, 2=Health
    type = rand() % 3;

    if (!texture.loadFromFile("sprites/PowerUp.png"))
    {
        // Fallback or error handling
    }

    sprite.setTexture(texture);

    // CHANGED: Increased scale from 0.1f to 0.25f (Bigger!)
    sprite.setScale(0.25f, 0.25f);

    // Color code based on type
    if (type == 0)
        sprite.setColor(sf::Color::Cyan);        // Invincible
    else if (type == 1)
        sprite.setColor(sf::Color(255, 165, 0)); // Orange (Rapid)
    else
        sprite.setColor(sf::Color::Green);       // Health

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
    // Adjusted hitbox shrinking for the bigger sprite
    float shrinkFactor = 0.3f;
    bounds.left += bounds.width * shrinkFactor / 2.f;
    bounds.top += bounds.height * shrinkFactor / 2.f;
    bounds.width *= (1.f - shrinkFactor);
    bounds.height *= (1.f - shrinkFactor);
    return bounds;
}