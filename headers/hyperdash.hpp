#ifndef HYPER_DASH_H
#define HYPER_DASH_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

// fwd declarations
class Player;
class Enemy;
class Obstacle;
class Coin;
class PowerUp;
class Spike;
class Projectile;

class GameState
{
private:
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Coin>> coins;
    std::vector<std::unique_ptr<PowerUp>> powerUp;
    std::unique_ptr<sf::RenderWindow> window;
    int score;
    bool isRunning;
    int gameState; // 0: GameOver, 1: Ingame

public:
    GameState();
    ~GameState() = default;

    void run();
    void update();
    void handleInput();
    void checkCollisions();
    void reset();

    Player *getPlayer() { return player.get(); }
    sf::RenderWindow *getWindow() { return window.get(); }
    int getScore() const { return score; }
    void addScore(int points) { score += points; }
};

// Player Class
class Player
{
private:
    sf::Texture texture;
    sf::Sprite sprite;

    int currentFrame;
    float animationTimer;
    float frameTime;
    int frameWidth;
    int frameHeight;
    int totalFrames;

    float speed;
    float damage;
    float jumpForce;
    bool isJumping;
    int health;
    sf::Vector2f velocity;

    bool isInvincible;
    float invincibilityTimer;
    float invincibilityDuration;

    std::vector<std::unique_ptr<Projectile>> projectiles;

public:
    Player();

    void move(float direction);
    void jump();
    void ground();
    void updateAnimation(float deltaTime);
    void update(float deltaTime);
    void shoot();
    void render(sf::RenderWindow *window);
    void takeDamage();

    sf::Sprite &getSprite() { return sprite; }
    int getHealth() const { return health; }
    bool getIsJumping() const { return isJumping; }
    std::vector<std::unique_ptr<Projectile>> &getProjectiles() { return projectiles; }

    // Invincibility methods
    void setInvincible(bool invincible, float duration = 0.0f);
    bool getIsInvincible() const { return isInvincible; }
};

// Enemy Class
class Enemy
{
private:
    sf::Sprite sprite;
    float speed;
    int health;
    int damage;
    std::vector<std::unique_ptr<Projectile>> projectiles;

public:
    Enemy();

    void move();
    void update();
    void render(sf::RenderWindow *window);
    void attack();
    void takeDamage(int dmg);

    sf::Sprite &getSprite() { return sprite; }
    int getDamage() const { return damage; }
    bool isAlive() const { return health > 0; }
    std::vector<std::unique_ptr<Projectile>> &getProjectiles() { return projectiles; }
};

// Obstacle
class Obstacle
{
protected:
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Texture texture;

public:
    Obstacle();
    virtual ~Obstacle() = default;

    virtual void update() = 0;
    virtual void render(sf::RenderWindow *window) = 0;
    virtual bool checkCollision(Player *player) = 0;

    sf::Sprite &getSprite() { return sprite; }
};

// Coin Class
class Coin
{
private:
    sf::Sprite sprite;
    sf::Texture texture;
    int value;
    float scrollSpeed;

public:
    Coin();

    void update();
    void render(sf::RenderWindow *window);
    void setScrollSpeed(float speed) { scrollSpeed = speed; }

    sf::Sprite &getSprite() { return sprite; }
    int getValue() const { return value; }
    sf::FloatRect getHitbox() const;
};

// PowerUp Class
class PowerUp
{
private:
    sf::Sprite sprite;
    sf::Texture texture;
    bool isActive;
    float duration;
    float scrollSpeed;

public:
    PowerUp();
    void spawn();
    void activate();
    void update();
    void render(sf::RenderWindow *window);
    void setScrollSpeed(float speed) { scrollSpeed = speed; }

    sf::Sprite &getSprite() { return sprite; }
    bool getIsActive() const { return isActive; }
    float getDuration() const { return duration; }
    sf::FloatRect getHitbox() const;
};

// Spike Class (Inherites from obstacle)
class Spike : public Obstacle
{
private:
    bool hasHit;
    float scrollSpeed;

public:
    Spike();

    void update() override;
    void setScrollSpeed(float speed) { scrollSpeed = speed; }
    float getScrollSpeed() const { return scrollSpeed; }
    void render(sf::RenderWindow *window) override;
    bool checkCollision(Player *player) override;
    bool getHasHit() const { return hasHit; }
    void setHasHit(bool hit) { hasHit = hit; }
    sf::FloatRect getHitbox() const;
};

// Projectile Class (Inherits from obstacle)
class Projectile : public Obstacle
{
private:
    float speed;
    sf::Vector2f direction;
    int damage;
    sf::Color projectileColor;

public:
    Projectile(int dmg, sf::Color color, float startX, float startY);

    void update() override;
    void render(sf::RenderWindow *window) override;
    bool checkCollision(Player *player) override;

    int getDamage() const { return damage; }
};
#endif