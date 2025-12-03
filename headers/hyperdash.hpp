#ifndef HYPER_DASH_H
#define HYPER_DASH_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include "screenObjects.hpp"

// fwd declarations
class Player;
class Enemy;
class Obstacle;
class Coin;
class PowerUp;
class Spike;
class Projectile;

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

// Projectile Class
class Projectile : public Obstacle
{
private:
    float speed;
    sf::Vector2f direction;
    int damage;
    bool fromPlayer;

public:
    Projectile(float x, float y, float dirX, float dirY, bool isFriendly);

    void update() override;
    void render(sf::RenderWindow *window) override;
    bool checkCollision(Player *player) override;
    bool checkCollision(Enemy *enemy);

    int getDamage() const { return damage; }
    bool isFriendly() const { return fromPlayer; }
    sf::FloatRect getHitbox() const;
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
    int maxHealth;
    sf::Vector2f velocity;
    
    bool isInvincible;
    float invincibilityTimer;
    float invincibilityDuration;

    std::vector<std::unique_ptr<Projectile>> projectiles;
    float shootCooldown;
    float currentShootCooldown; 
    float shootTimer;
    
    bool isRapidFire;
    float rapidFireTimer;
    float rapidFireDuration;

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
    void heal(int amount); 

    sf::Sprite &getSprite() { return sprite; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    bool getIsJumping() const { return isJumping; }
    std::vector<std::unique_ptr<Projectile>>& getProjectiles() { return projectiles; }

    void setInvincible(bool invincible, float duration = 0.0f);
    bool getIsInvincible() const { return isInvincible; }

    void setRapidFire(bool enable, float duration = 0.0f); 
    bool getIsRapidFire() const { return isRapidFire; }
};

// Enemy Class
class Enemy
{
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    float speed;
    int health;
    int damage;
    std::vector<std::unique_ptr<Projectile>> projectiles;
    float shootTimer;

public:
    Enemy();
    virtual ~Enemy() = default;

    virtual void update(float deltaTime, float scrollSpeed) = 0; 
    virtual void render(sf::RenderWindow *window);
    virtual void takeDamage(int dmg);

    sf::Sprite &getSprite() { return sprite; }
    int getDamage() const { return damage; }
    bool isAlive() const { return health > 0; }
    std::vector<std::unique_ptr<Projectile>> &getProjectiles() { return projectiles; }
    sf::FloatRect getHitbox() const;
};

class Walker : public Enemy
{
public:
    Walker(float startX, float startY);
    void update(float deltaTime, float scrollSpeed) override;
};

class Drone : public Enemy
{
private:
    float startY;
    float timeAlive;
public:
    Drone(float startX, float startY);
    void update(float deltaTime, float scrollSpeed) override;
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
    int type;

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
    int getType() const { return type; } 
    sf::FloatRect getHitbox() const;
};

// Spike Class
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
    int highScore;
    bool isRunning;
    int gameState; // 0: GameOver, 1: Ingame, 2: Menu
    int difficulty; 

    bool debugMode;
    sf::Text debugText;
    sf::Font debugFont;

    StartScreen startScreen;
    StatusHUD statusHUD;
    
    sf::SoundBuffer shootBuffer;
    sf::SoundBuffer jumpBuffer;
    sf::SoundBuffer collectBuffer;
    sf::SoundBuffer hitBuffer;
    sf::Sound shootSound;
    sf::Sound jumpSound;
    sf::Sound collectSound;
    sf::Sound hitSound;

    float coinSpawnTimer;
    float spikeSpawnTimer;
    float powerUpSpawnTimer;
    float enemySpawnTimer;

public:
    GameState();
    ~GameState() = default;

    void run();
    void update(); 
    void renderMenu(); 
    void handleInput();
    void checkCollisions();
    void reset();

    void loadHighScore();
    void saveHighScore();

    Player *getPlayer() { return player.get(); }
    sf::RenderWindow *getWindow() { return window.get(); }
    int getScore() const { return score; }
    void addScore(int points) { score += points; }
    
    void playSound(sf::Sound& sound);
};

#endif