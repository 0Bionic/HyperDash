#include "screenObjects.hpp"
#include <iostream>
#include <cmath>

// =========================================================
// BACKGROUND
// =========================================================
Background::Background(const std::string &texturePath, float speed)
    : scrollSpeed(speed)
{
    if (!texture.loadFromFile(texturePath))
    {
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

// =========================================================
// HEALTH BAR
// =========================================================
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

// =========================================================
// SCORE DISPLAY
// =========================================================
ScoreDisplay::ScoreDisplay(float x, float y)
    : currentScore(0), position(x, y)
{
    if (!font.loadFromFile("fonts/PressStart2P-Regular.ttf"))
    {
    }

    scoreLabel.setFont(font);
    scoreLabel.setString("Score:");
    scoreLabel.setCharacterSize(24);
    scoreLabel.setFillColor(sf::Color::White);
    scoreLabel.setPosition(x, y);

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
    window->draw(scoreLabel);
    window->draw(scoreText);
}

void ScoreDisplay::setPosition(float x, float y)
{
    position.x = x;
    position.y = y;
    scoreLabel.setPosition(x, y);
    scoreText.setPosition(x + 80, y - 4);
}

// =========================================================
// GAME OVER SCREEN
// =========================================================
GameOverScreen::GameOverScreen()
{
    if (!backgroundTexture.loadFromFile("sprites/GameOver.png"))
    {
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setPosition(0, 0);

    if (!font.loadFromFile("fonts/PressStart2P-Regular.ttf"))
    {
    }

    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    sf::FloatRect r = gameOverText.getLocalBounds();
    gameOverText.setOrigin(r.left + r.width/2.0f, r.top + r.height/2.0f);
    gameOverText.setPosition(640, 250);

    restartText.setFont(font);
    restartText.setString("PRESS ENTER TO RESTART");
    restartText.setCharacterSize(20);
    restartText.setFillColor(sf::Color::White);
    r = restartText.getLocalBounds();
    restartText.setOrigin(r.left + r.width/2.0f, r.top + r.height/2.0f);
    restartText.setPosition(640, 450);

    blinkTimer = 0.0f;
    showRestartText = true;
}

void GameOverScreen::update(float deltaTime)
{
    blinkTimer += deltaTime;
    if (blinkTimer >= 0.5f)
    {
        blinkTimer = 0.0f;
        showRestartText = !showRestartText;
    }
}

void GameOverScreen::render(sf::RenderWindow *window)
{
    window->draw(backgroundSprite);
    window->draw(gameOverText);
    if (showRestartText)
    {
        window->draw(restartText);
    }
}

// =========================================================
// START SCREEN (NEW IMPL)
// =========================================================
StartScreen::StartScreen() : menuTimer(0.0f)
{
    if (!font.loadFromFile("fonts/PressStart2P-Regular.ttf"))
    {
        std::cout << "Failed to load font in StartScreen" << std::endl;
    }

    // Title
    titleText.setFont(font);
    titleText.setString("HYPER DASH");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::Cyan);
    sf::FloatRect tr = titleText.getLocalBounds();
    titleText.setOrigin(tr.left + tr.width/2.0f, tr.top + tr.height/2.0f);
    titleText.setPosition(640, 200);

    // High Score
    highScoreText.setFont(font);
    highScoreText.setCharacterSize(20);
    highScoreText.setFillColor(sf::Color(255, 215, 0)); // Gold
    highScoreText.setPosition(640, 260);

    // Start
    startText.setFont(font);
    startText.setString("PRESS ENTER TO START");
    startText.setCharacterSize(24);
    tr = startText.getLocalBounds();
    startText.setOrigin(tr.left + tr.width/2.0f, tr.top + tr.height/2.0f);
    startText.setPosition(640, 500);

    // Difficulty
    difficultyText.setFont(font);
    difficultyText.setCharacterSize(24);
    difficultyText.setFillColor(sf::Color::Yellow);

    // Controls
    controlsText.setFont(font);
    controlsText.setString("ARROWS: Move | HOLD SPACE: Shoot | ~: Debug");
    controlsText.setCharacterSize(15);
    controlsText.setFillColor(sf::Color(150, 150, 150));
    tr = controlsText.getLocalBounds();
    controlsText.setOrigin(tr.left + tr.width/2.0f, tr.top + tr.height/2.0f);
    controlsText.setPosition(640, 600);
}

void StartScreen::update(float deltaTime)
{
    menuTimer += deltaTime;
}

void StartScreen::render(sf::RenderWindow *window, int difficulty, int highScore)
{
    window->clear(sf::Color::Black);

    window->draw(titleText);

    // High Score
    highScoreText.setString("HIGH SCORE: " + std::to_string(highScore));
    sf::FloatRect hr = highScoreText.getLocalBounds();
    highScoreText.setOrigin(hr.left + hr.width/2.0f, hr.top + hr.height/2.0f);
    highScoreText.setPosition(640, 260);
    window->draw(highScoreText);

    // Blinking Start Text
    int alpha = (int)(127.5f * (1.0f + std::sin(menuTimer * 5.0f)));
    startText.setFillColor(sf::Color(255, 255, 255, alpha));
    window->draw(startText);

    window->draw(controlsText);

    // Difficulty
    std::string diffStr;
    switch (difficulty)
    {
    case 0:
        diffStr = "EASY";
        break;
    case 1:
        diffStr = "NORMAL";
        break;
    case 2:
        diffStr = "HARD";
        break;
    }
    difficultyText.setString("< " + diffStr + " >");
    sf::FloatRect textRect = difficultyText.getLocalBounds();
    difficultyText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    difficultyText.setPosition(640, 400);
    window->draw(difficultyText);

    window->display();
}

// =========================================================
// STATUS HUD (NEW IMPL)
// =========================================================
StatusHUD::StatusHUD()
{
    if (!font.loadFromFile("fonts/PressStart2P-Regular.ttf"))
    {
    }

    invincibleText.setFont(font);
    invincibleText.setString("INVINCIBLE");
    invincibleText.setCharacterSize(18);
    invincibleText.setFillColor(sf::Color::Cyan);

    rapidFireText.setFont(font);
    rapidFireText.setString("RAPID FIRE");
    rapidFireText.setCharacterSize(18);
    rapidFireText.setFillColor(sf::Color(255, 165, 0));
}

void StatusHUD::render(sf::RenderWindow *window, bool isInvincible, bool isRapidFire)
{
    float statusY = 80.0f;

    if (isInvincible)
    {
        invincibleText.setPosition(30, statusY);
        window->draw(invincibleText);
        statusY += 30.0f;
    }

    if (isRapidFire)
    {
        rapidFireText.setPosition(30, statusY);
        window->draw(rapidFireText);
    }
}