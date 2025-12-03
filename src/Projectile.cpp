#include "hyperdash.hpp"
#include <iostream>

Projectile::Projectile(int dmg, sf::Color color, float startX, float startY)
{
    damage = dmg;
    speed = 500.0f;
    direction = sf::Vector2f(1.0f, 0.0f);
    projectileColor = color;
    position = sf::Vector2f(startX, startY);

    if (!texture.loadFromFile("sprites/FireBall.png"))
    {
        std::cout << "Failed to load projectile sprite!" << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setScale(0.05f, 0.05f);
    sprite.setColor(projectileColor);
    sprite.setPosition(position);
}

void Projectile::update()
{
    float deltaTime = 0.016f;
    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;
    sprite.setPosition(position);
}

void Projectile::render(sf::RenderWindow *window)
{
    window->draw(sprite);
}

bool Projectile::checkCollision(Player *player)
{
    return sprite.getGlobalBounds().intersects(player->getSprite().getGlobalBounds());
}