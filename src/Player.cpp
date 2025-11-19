#include "hyperdash.hpp"

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
    jumpForce = -500.0f;
    isJumping = false;
    health = 9;
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

void Player::update()
{
    // Apply gravity
    velocity.y += 1500.0f * 0.016f;

    sf::Vector2f pos = sprite.getPosition();
    pos.y += velocity.y * 0.016f;
    sprite.setPosition(pos);

    // Ground check
    if (pos.y >= 400)
    {
        pos.y = 400;
        velocity.y = 0.0f;
        isJumping = false;
        sprite.setPosition(pos);
    }
}

void Player::render(sf::RenderWindow *window)
{
    window->draw(sprite);
}

void Player::takeDamage()
{
    health--;
    if (health < 0)
        health = 0;
}