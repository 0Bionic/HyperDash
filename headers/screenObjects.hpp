#ifndef SCREENOBJECT_H
#define SCREENOBJECT_H

#include <SFML/Graphics.hpp>
#include <string>

class Background
{
private:
    sf::Texture texture;
    sf::Sprite sprite1;
    sf::Sprite sprite2;
    float scrollSpeed;

public:
    Background(const std::string &texturePath, float speed);

    void update();
    void render(sf::RenderWindow *window);
    void setSpeed(float speed);
    float getSpeed() const { return scrollSpeed; }
};

class HealthBar
{
private:
    sf::RectangleShape background;
    sf::RectangleShape bar;
    sf::Font font;
    sf::Text label;
    float maxWidth;
    int maxHealth;

public:
    HealthBar(float x, float y, float width, float height, int maxHealth);

    void update(int currentHealth);
    void render(sf::RenderWindow *window);
};

class ScoreDisplay
{
private:
    sf::Font font;
    sf::Text scoreText;
    sf::Text scoreLabel;
    sf::RectangleShape background;
    int currentScore;
    sf::Vector2f position;

public:
    ScoreDisplay(float x, float y);

    void update(int score);
    void render(sf::RenderWindow *window);
    void setPosition(float x, float y);
    int getScore() const { return currentScore; }
};

class GameOverScreen
{
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Font font;
    sf::Text gameOverText;
    sf::Text restartText;
    float blinkTimer;
    bool showRestartText;

public:
    GameOverScreen();

    void update(float deltaTime);
    void render(sf::RenderWindow *window);
};
#endif