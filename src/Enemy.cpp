#include "hyperdash.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

Enemy::Enemy()
    : health(1), damage(1), shootTimer(0.0f)
{
}

void Enemy::render(sf::RenderWindow *window)
{
    window->draw(sprite);

    for (const auto &proj : projectiles)
    {
        proj->render(window);
    }
}

void Enemy::takeDamage(int dmg)
{
    health -= dmg;
}

sf::FloatRect Enemy::getHitbox() const
{
    return sprite.getGlobalBounds();
}

Walker::Walker(float startX, float startY)
{
    if (!texture.loadFromFile("sprites/walker.png"))
    {
        std::cout << "Failed to load sprites/walker.png" << std::endl;
        sf::Image img;
        img.create(50, 50, sf::Color::Red);
        texture.loadFromImage(img);
    }

    sprite.setTexture(texture);
    sprite.setPosition(startX, startY);

    sprite.setScale(1.0f, 1.0f);

    speed = 100.0f;
    health = 2;
    damage = 1;
    shootTimer = 0.0f;
}

void Walker::update(float deltaTime, float scrollSpeed)
{
    sprite.move(-(scrollSpeed * 60.0f * deltaTime) - (speed * deltaTime), 0);

    shootTimer += deltaTime;
    if (shootTimer >= 2.0f)
    {
        shootTimer = 0.0f;

        float spawnX = sprite.getPosition().x;
        float spawnY = sprite.getPosition().y + (sprite.getGlobalBounds().height / 2);

        projectiles.push_back(std::make_unique<Projectile>(spawnX, spawnY, -1.0f, 0.0f, false));
    }

    for (auto &proj : projectiles)
    {
        proj->update();
    }

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
                       [](const std::unique_ptr<Projectile> &p)
                       {
                           float x = p->getSprite().getPosition().x;
                           return (x < -50 || x > 1300);
                       }),
        projectiles.end());
}

Drone::Drone(float startX, float startY)
{
    if (!texture.loadFromFile("sprites/drone.png"))
    {
        std::cout << "Failed to load sprites/drone.png" << std::endl;
        sf::Image img;
        img.create(40, 40, sf::Color::Magenta);
        texture.loadFromImage(img);
    }

    sprite.setTexture(texture);
    sprite.setPosition(startX, startY);
    sprite.setScale(1.0f, 1.0f);

    this->startY = startY;
    this->timeAlive = 0.0f;

    speed = 150.0f;
    health = 1;
    damage = 1;
}

void Drone::update(float deltaTime, float scrollSpeed)
{
    timeAlive += deltaTime;

    float yOffset = std::sin(timeAlive * 3.0f) * 100.0f * deltaTime;

    sprite.move(-(scrollSpeed * 60.0f * deltaTime) - (speed * deltaTime), yOffset);

    shootTimer += deltaTime;
    if (shootTimer >= 1.5f)
    {
        shootTimer = 0.0f;

        float spawnX = sprite.getPosition().x;
        float spawnY = sprite.getPosition().y + (sprite.getGlobalBounds().height / 2);

        projectiles.push_back(std::make_unique<Projectile>(spawnX, spawnY, -1.5f, 0.3f, false));
    }

    for (auto &proj : projectiles)
    {
        proj->update();
    }

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
                       [](const std::unique_ptr<Projectile> &p)
                       {
                           float x = p->getSprite().getPosition().x;
                           return (x < -50 || x > 1300);
                       }),
        projectiles.end());
}