#include "hyperdash.hpp"
#include <iostream>

Projectile::Projectile(float x, float y, float dirX, float dirY, bool isFriendly)
{
    fromPlayer = isFriendly;
    direction = sf::Vector2f(dirX, dirY);
    speed = 800.0f;
    damage = 1;

    if (!texture.loadFromFile("sprites/bullet.png"))
    {
        std::cout << "Error loading bullet.png" << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setScale(1.0f, 1.0f);

    sprite.setPosition(x, y);

    if (texture.getSize().x == 0)
    {
        sprite.setTextureRect(sf::IntRect(0, 0, 10, 5));
        sprite.setColor(isFriendly ? sf::Color::Yellow : sf::Color::Red);
    }
    else
    {
        sprite.setScale(0.5f, 0.5f);
        sprite.setColor(isFriendly ? sf::Color::Yellow : sf::Color::Red);
    }
}

void Projectile::update()
{
    sprite.move(direction.x * speed * 0.016f, direction.y * speed * 0.016f);
}

void Projectile::render(sf::RenderWindow *window)
{
    window->draw(sprite);
}

bool Projectile::checkCollision(Player *player)
{
    if (fromPlayer)
        return false;

    return getHitbox().intersects(player->getSprite().getGlobalBounds());
}

bool Projectile::checkCollision(Enemy *enemy)
{
    if (!fromPlayer)
        return false;

    return getHitbox().intersects(enemy->getHitbox());
}

sf::FloatRect Projectile::getHitbox() const
{
    return sprite.getGlobalBounds();
}