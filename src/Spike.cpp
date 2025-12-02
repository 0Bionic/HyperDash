#include "hyperdash.hpp"
#include <cstdlib>
#include <iostream>

Obstacle::Obstacle()
{
    position = sf::Vector2f(0, 0);
}

Spike::Spike()
{
    hasHit = false;
    scrollSpeed = 2.0f;

    if (!texture.loadFromFile("sprites/spike.png"))
    {
        std::cout << "Failed to load spike sprite!" << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setScale(0.25f, 0.25f);

    float randomX = 1280 + static_cast<float>(rand() % 300);
    float groundY = 513;

    position = sf::Vector2f(randomX, groundY);
    sprite.setPosition(position);
}

void Spike::update()
{
    sprite.move(-scrollSpeed, 0.0f);
    position = sprite.getPosition();
}

void Spike::render(sf::RenderWindow *window)
{
    window->draw(sprite);
}

sf::FloatRect Spike::getHitbox() const
{
    sf::FloatRect bounds = sprite.getGlobalBounds();

    // Shrink hitbox for more forgiving collision detection
    float shrinkFactor = 0.4f;
    bounds.left += bounds.width * shrinkFactor / 2.0f;
    bounds.top += bounds.height * shrinkFactor / 2.0f;
    bounds.width *= (1.0f - shrinkFactor);
    bounds.height *= (1.0f - shrinkFactor);

    return bounds;
}

bool Spike::checkCollision(Player *player)
{
    return getHitbox().intersects(player->getSprite().getGlobalBounds());
}