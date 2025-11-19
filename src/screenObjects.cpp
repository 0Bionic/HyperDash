#include "screenObjects.hpp"

// Background Rendering

Background::Background(const std::string &texturePath, float speed)
    : scrollSpeed(speed)
{
    if (!texture.loadFromFile(texturePath))
    {
        // Handle error - could throw exception or log
    }

    sprite1.setTexture(texture);
    sprite2.setTexture(texture);

    sprite1.setPosition(0, 0);
    sprite2.setPosition(sprite1.getGlobalBounds().width, 0);
}

void Background::update()
{
    sprite1.move(-scrollSpeed, 0);
    sprite2.move(-scrollSpeed, 0);

    if (sprite1.getPosition().x + sprite1.getGlobalBounds().width < 0)
    {
        sprite1.setPosition(sprite2.getPosition().x + sprite2.getGlobalBounds().width, 0);
    }

    if (sprite2.getPosition().x + sprite2.getGlobalBounds().width < 0)
    {
        sprite2.setPosition(sprite1.getPosition().x + sprite1.getGlobalBounds().width, 0);
    }
}

void Background::render(sf::RenderWindow *window)
{
    window->draw(sprite1);
    window->draw(sprite2);
}

void Background::setSpeed(float speed)
{
    scrollSpeed = speed;
}

// Health Bar rendering

HealthBar::HealthBar(float x, float y, float width, float height, int maxHealth)
    : maxWidth(width), maxHealth(maxHealth)
{
    background.setSize(sf::Vector2f(width, height));
    background.setPosition(x, y);
    background.setFillColor(sf::Color(50, 50, 50));
    background.setOutlineColor(sf::Color::White);
    background.setOutlineThickness(2);

    bar.setSize(sf::Vector2f(width, height));
    bar.setPosition(x, y);
    bar.setFillColor(sf::Color::Green);

    if (!font.loadFromFile("fonts/PressStart2P-Regular.ttf"))
    {
        // Handle font load error
    }

    label.setFont(font);
    label.setString("HEALTH:");
    label.setCharacterSize(16);
    label.setFillColor(sf::Color::White);
    label.setPosition(x, y - 20);
}

void HealthBar::update(int currentHealth)
{
    float healthPercent = static_cast<float>(currentHealth) / maxHealth;
    bar.setSize(sf::Vector2f(maxWidth * healthPercent, bar.getSize().y));

    if (healthPercent > 0.6f)
        bar.setFillColor(sf::Color::Green);
    else if (healthPercent > 0.3f)
        bar.setFillColor(sf::Color::Yellow);
    else
        bar.setFillColor(sf::Color::Red);
}

void HealthBar::render(sf::RenderWindow *window)
{
    window->draw(label);
    window->draw(background);
    window->draw(bar);
}