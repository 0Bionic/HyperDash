#include "hyperdash.hpp"
#include <cmath>

Player::Player()
{
    if (!texture.loadFromFile("sprites/spritesheetvolt_run.png"))
    {
    }

    sprite.setTexture(texture);
    sf::Vector2u textureSize = texture.getSize();
    frameWidth = textureSize.x / 5;
    frameHeight = textureSize.y / 2;
    totalFrames = 5;
    currentFrame = 0;
    animationTimer = 0.0f;
    frameTime = 0.1f;

    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    sprite.setPosition(100, 400);
    sprite.setScale(0.5f, 0.5f);

    speed = 500.0f;
    jumpForce = -1000.0f;
    isJumping = false;
    maxHealth = 9;
    health = maxHealth;
    velocity = sf::Vector2f(0.0f, 0.0f);

    isInvincible = false;
    invincibilityTimer = 0.0f;
    invincibilityDuration = 0.0f;

    // Rapid Fire
    isRapidFire = false;
    rapidFireTimer = 0.0f;
    rapidFireDuration = 0.0f;

    // Shooting
    shootCooldown = 0.4f; // Normal Rate
    currentShootCooldown = shootCooldown;
    shootTimer = 0.0f;
}

void Player::updateAnimation(float deltaTime)
{
    animationTimer += deltaTime;
    if (animationTimer >= frameTime)
    {
        animationTimer = 0.0f;
        currentFrame = (currentFrame + 1) % totalFrames;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
    }
}

void Player::move(float direction)
{
    velocity.x += direction * speed;
}

void Player::jump()
{
    if (!isJumping)
    {
        velocity.y = jumpForce;
        isJumping = true;
    }
}

void Player::ground()
{
    if (isJumping)
    {
        sf::Vector2f pos = sprite.getPosition();
        pos.y = 400;
        velocity.y = 0.0f;
        isJumping = false;
        sprite.setPosition(pos);
    }
}

void Player::shoot()
{
    // Check if cooldown is ready
    if (shootTimer <= 0.0f)
    {
        float spawnX = sprite.getPosition().x + (frameWidth * 0.5f);
        float spawnY = sprite.getPosition().y + (frameHeight * 0.25f);

        projectiles.push_back(std::make_unique<Projectile>(spawnX, spawnY, 1.0f, 0.0f, true));

        // Reset timer based on current mode (Rapid or Normal)
        shootTimer = currentShootCooldown;
    }
}

void Player::heal(int amount)
{
    health += amount;
    if (health > maxHealth)
    {
        health = maxHealth;
    }
}

void Player::update(float deltaTime)
{
    if (shootTimer > 0.0f)
    {
        shootTimer -= deltaTime;
    }

    // Physics
    velocity.y += 2500.0f * deltaTime;
    sf::Vector2f pos = sprite.getPosition();
    pos.x += velocity.x * deltaTime;
    pos.y += velocity.y * deltaTime;

    if (pos.x < 0)
        pos.x = 0;
    if (pos.x > 1280 - frameWidth * 0.5f)
        pos.x = 1280 - frameWidth * 0.5f;
    if (pos.y >= 400)
    {
        pos.y = 400;
        velocity.y = 0.0f;
        isJumping = false;
    }
    sprite.setPosition(pos);
    velocity.x = 0.0f;

    // Handle Powerups
    if (isInvincible)
    {
        invincibilityTimer += deltaTime;
        if (invincibilityTimer >= invincibilityDuration)
        {
            setInvincible(false);
        }
    }

    if (isRapidFire)
    {
        rapidFireTimer += deltaTime;
        if (rapidFireTimer >= rapidFireDuration)
        {
            setRapidFire(false);
        }
    }

    // Update Bullets
    for (auto it = projectiles.begin(); it != projectiles.end();)
    {
        (*it)->update();
        if ((*it)->getSprite().getPosition().x > 1300)
        {
            it = projectiles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Player::render(sf::RenderWindow *window)
{
    window->draw(sprite);
    for (auto &proj : projectiles)
    {
        proj->render(window);
    }
}

void Player::takeDamage()
{
    if (isInvincible)
        return;
    health--;
    if (health < 0)
        health = 0;
}

void Player::setInvincible(bool invincible, float duration)
{
    isInvincible = invincible;
    invincibilityDuration = duration;
    invincibilityTimer = 0.0f;
    // Visual: Cyan if invincible
    if (invincible)
    {
        sprite.setColor(sf::Color(0, 255, 255));
    }
    else if (!isRapidFire)
    {
        sprite.setColor(sf::Color::White); // Reset unless RapidFire is active
    }
}

void Player::setRapidFire(bool enable, float duration)
{
    isRapidFire = enable;
    rapidFireDuration = duration;
    rapidFireTimer = 0.0f;

    if (enable)
    {
        currentShootCooldown = 0.15f; // FAST firing speed
        sprite.setColor(sf::Color(255, 165, 0)); // Orange tint
    }
    else
    {
        currentShootCooldown = shootCooldown; // Return to normal
        if (!isInvincible)
        {
            sprite.setColor(sf::Color::White); // Reset unless Invincible
        }
    }
}