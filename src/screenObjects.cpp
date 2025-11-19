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

// Scoring

ScoreDisplay::ScoreDisplay(float x, float y)
    : currentScore(0), position(x, y)
{

    if (!font.loadFromFile("fonts/PressStart2P-Regular.ttf"))
    {
    }

    // Setup score label (e.g., "Score:")
    scoreLabel.setFont(font);
    scoreLabel.setString("Score:");
    scoreLabel.setCharacterSize(24);
    scoreLabel.setFillColor(sf::Color::White);
    scoreLabel.setPosition(x, y);

    // Setup score text (the actual number)
    scoreText.setFont(font);
    scoreText.setString("0");
    scoreText.setCharacterSize(26);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setPosition(x + 140, y);
}

void ScoreDisplay::update(int score)
{
    currentScore = score;
    scoreText.setString(std::to_string(currentScore));
}

void ScoreDisplay::render(sf::RenderWindow *window)
{
    window->draw(background);
    window->draw(scoreLabel);
    window->draw(scoreText);
}

void ScoreDisplay::setPosition(float x, float y)
{
    position.x = x;
    position.y = y;

    scoreLabel.setPosition(x, y);
    scoreText.setPosition(x + 80, y - 4);
    background.setPosition(x - 10, y - 5);
}