#include "hyperdash.hpp"
#include <cstdlib>
#include <iostream>

Obstacle::Obstacle()
{
    position = sf::Vector2f(0, 0);
}
Spike::Spike()
{
    if (!texture.loadFromFile("sprites/spike.png"))
    {
        std::cout << "Failed to load spike sprite!" << std::endl;
    }

    sprite.setTexture(texture);

    // Scale up the spike to make it bigger
    sprite.setScale(0.25f, 0.25f);

    // Spawn at ground level - need to position bottom of spike at y=400
    float randomX = 1280 + static_cast<float>(rand() % 300);
    // sf::FloatRect bounds = sprite.getLocalBounds();
    float groundY = 513; // Account for scale

    position = sf::Vector2f(randomX, groundY);
    sprite.setPosition(position);
}

void Spike::update()
{
    sprite.move(-2.0f, 0.0f);
    position = sprite.getPosition();
}

void Spike::render(sf::RenderWindow *window)
{
    window->draw(sprite);
}

bool Spike::checkCollision(Player *player)
{
    return sprite.getGlobalBounds().intersects(player->getSprite().getGlobalBounds());
}