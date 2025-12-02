#include "hyperdash.hpp"
#include <cmath>

Player::Player()
{
    if (!texture.loadFromFile("sprites/spritesheetvolt_run.png"))
    {
        // Fallback: create a simple colored rectangle if texture fails
    }

    sprite.setTexture(texture);

    // Get actual texture size and calculate frame dimensions
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

    speed = 300.0f;
    jumpForce = -700.0f;
    isJumping = false;
    health = 9;

    // Invincibility initialization
    isInvincible = false;
    invincibilityTimer = 0.0f;
    invincibilityDuration = 0.0f;
}

void Player::updateAnimation(float deltaTime)
{
    animationTimer += deltaTime;

    if (animationTimer >= frameTime)
    {
        animationTimer = 0.0f;
        currentFrame = (currentFrame + 1) % totalFrames;

        sprite.setTextureRect(sf::IntRect(
            currentFrame * frameWidth,
            0,
            frameWidth,
            frameHeight));
    }
}

void Player::move(float direction)
{
    sf::Vector2f pos = sprite.getPosition();
    pos.x += direction * speed * 0.016f;

    // Keep player on screen
    if (pos.x < 0)
        pos.x = 0;
    if (pos.x > 1280 - frameWidth * 0.5f)
        pos.x = 1280 - frameWidth * 0.5f;

    sprite.setPosition(pos);
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

void Player::update(float deltaTime)
{
    // Apply gravity
    velocity.y += 1500.0f * deltaTime;

    sf::Vector2f pos = sprite.getPosition();
    pos.y += velocity.y * deltaTime;
    sprite.setPosition(pos);

    // Ground check
    if (pos.y >= 400)
    {
        pos.y = 400;
        velocity.y = 0.0f;
        isJumping = false;
        sprite.setPosition(pos);
    }

    // Handle invincibility
    if (isInvincible)
    {
        invincibilityTimer += deltaTime;

        // Check if invincibility has expired
        if (invincibilityTimer >= invincibilityDuration)
        {
            setInvincible(false);
        }
    }
}

void Player::render(sf::RenderWindow *window)
{
    window->draw(sprite);
}

void Player::takeDamage()
{
    // Don't take damage if invincible
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

    if (invincible)
    {
        // Apply cyan tint
        sprite.setColor(sf::Color(0, 255, 255, 255));
    }
    else
    {
        // Reset to normal color
        sprite.setColor(sf::Color(255, 255, 255, 255));
    }
}