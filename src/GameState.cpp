#include "hyperdash.hpp"
#include "screenObjects.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <fstream>

GameState::GameState()
    : score(0), highScore(0), isRunning(true), gameState(2), difficulty(1), debugMode(false)
{
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1280, 720), "HyperDash");
    window->setFramerateLimit(60);

    loadHighScore();

    // Debug Font
    if (!debugFont.loadFromFile("fonts/PressStart2P-Regular.ttf"))
    {
    }
    debugText.setFont(debugFont);
    debugText.setCharacterSize(12);
    debugText.setFillColor(sf::Color::Red);
    debugText.setPosition(10, 10);
    debugText.setString("DEBUG MODE ON\n1: Coin\n2: Spike\n3: Walker\n4: Drone\n5: PowerUp\n0: God Mode");

    // Audio
    if (shootBuffer.loadFromFile("sounds/shoot.wav"))
        shootSound.setBuffer(shootBuffer);
    if (jumpBuffer.loadFromFile("sounds/jump.wav"))
        jumpSound.setBuffer(jumpBuffer);
    if (collectBuffer.loadFromFile("sounds/collect.wav"))
        collectSound.setBuffer(collectBuffer);
    if (hitBuffer.loadFromFile("sounds/hit.wav"))
        hitSound.setBuffer(hitBuffer);
    shootSound.setVolume(50);

    player = std::make_unique<Player>();
    srand(static_cast<unsigned>(time(0)));
    reset();
}

void GameState::loadHighScore()
{
    std::ifstream inputFile("highscore.txt");
    if (inputFile.is_open())
    {
        inputFile >> highScore;
        inputFile.close();
    }
    else
    {
        highScore = 0;
    }
}

void GameState::saveHighScore()
{
    std::ofstream outputFile("highscore.txt");
    if (outputFile.is_open())
    {
        outputFile << highScore;
        outputFile.close();
    }
}

void GameState::playSound(sf::Sound &sound)
{
    if (sound.getBuffer() != NULL)
    {
        sound.setPitch(0.9f + static_cast<float>(rand() % 20)/100.0f);
        sound.play();
    }
}

void GameState::run()
{
    float baseScrollSpeed = 2.0f;
    Background background("sprites/background.png", baseScrollSpeed);
    HealthBar healthBar(1000, 40, 250, 20, 9);
    ScoreDisplay scoreDisplay(30, 40);
    GameOverScreen gameOverScreen;
    sf::Clock clock;
    sf::Clock gameTimer;
    float scoreIncreaseTimer = 0.0f;

    while (isRunning && window->isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        handleInput();

        // DELEGATE TO StartScreen
        if (gameState == 2)
        {
            startScreen.update(deltaTime);
            startScreen.render(window.get(), difficulty, highScore);
            continue;
        }

        if (player->getHealth() <= 0)
        {
            if (score > highScore)
            {
                highScore = score;
                saveHighScore();
            }
            gameState = 0;
        }

        if (gameState == 0)
        {
            gameOverScreen.update(deltaTime);
            window->clear(sf::Color::Black);
            gameOverScreen.render(window.get());

            // Optionally render high score on game over here if desired,
            // or add to GameOverScreen class later.
            window->display();
            continue;
        }

        // Difficulty Settings
        float elapsedTime = gameTimer.getElapsedTime().asSeconds();
        float spawnMult = 1.0f;
        if (difficulty == 0)
            spawnMult = 1.5f;
        if (difficulty == 2)
            spawnMult = 0.6f;

        int spikeChance = 40;
        int enemyChance = 50;
        if (difficulty == 0)
        {
            spikeChance = 25;
            enemyChance = 30;
        }
        if (difficulty == 2)
        {
            spikeChance = 70;
            enemyChance = 80;
        }

        float speedRamp = (difficulty == 2) ? 30.0f : 60.0f;
        float currentSpeed = baseScrollSpeed * (1.0f + (elapsedTime / speedRamp));

        // Logic Updates
        player->update(deltaTime);
        player->updateAnimation(deltaTime);
        background.setSpeed(currentSpeed);
        background.update();
        healthBar.update(player->getHealth());
        scoreDisplay.update(score);

        // --- SPAWNING ---
        coinSpawnTimer += deltaTime;
        if (coinSpawnTimer >= 2.0f)
        {
            coinSpawnTimer = 0.0f;
            if (rand() % 100 < 30)
            {
                auto coin = std::make_unique<Coin>();
                coin->setScrollSpeed(currentSpeed);
                coins.push_back(std::move(coin));
            }
        }

        spikeSpawnTimer += deltaTime;
        if (spikeSpawnTimer >= 1.5f * spawnMult)
        {
            spikeSpawnTimer = 0.0f;
            if (rand() % 100 < spikeChance)
            {
                auto spike = std::make_unique<Spike>();
                spike->setScrollSpeed(currentSpeed);
                obstacles.push_back(std::move(spike));
            }
        }

        powerUpSpawnTimer += deltaTime;
        if (powerUpSpawnTimer >= 6.0f)
        {
            powerUpSpawnTimer = 0.0f;
            if (rand() % 100 < 50)
            {
                auto newPowerUp = std::make_unique<PowerUp>();
                newPowerUp->setScrollSpeed(currentSpeed);
                powerUp.push_back(std::move(newPowerUp));
            }
        }

        enemySpawnTimer += deltaTime;
        if (enemySpawnTimer >= 4.0f * spawnMult)
        {
            enemySpawnTimer = 0.0f;
            if (rand() % 100 < enemyChance)
            {
                int type = rand() % 2;
                if (type == 0)
                    enemies.push_back(std::make_unique<Walker>(1300.0f, 400.0f));
                else
                {
                    float airY = 200.0f + static_cast<float>(rand() % 150);
                    enemies.push_back(std::make_unique<Drone>(1300.0f, airY));
                }
            }
        }

        scoreIncreaseTimer += deltaTime;
        if (scoreIncreaseTimer >= 0.75f)
        {
            scoreIncreaseTimer = 0.0f;
            addScore(10);
        }

        for (auto &c : coins)
        {
            c->setScrollSpeed(currentSpeed);
            c->update();
        }
        for (auto &o : obstacles)
        {
            Spike *s = dynamic_cast<Spike *>(o.get());
            if (s)
                s->setScrollSpeed(currentSpeed);
            o->update();
        }
        for (auto &e : enemies)
        {
            e->update(deltaTime, currentSpeed);
        }
        for (auto &p : powerUp)
        {
            p->setScrollSpeed(currentSpeed);
            p->update();
        }

        checkCollisions();

        auto remover = [](const auto &entity)
        { return entity->getSprite().getPosition().x < -100; };
        coins.erase(std::remove_if(coins.begin(), coins.end(), remover), coins.end());
        obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), remover), obstacles.end());
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                     [](const auto &en)
                                     { return en->getSprite().getPosition().x < -100 || !en->isAlive(); }),
                      enemies.end());
        powerUp.erase(std::remove_if(powerUp.begin(), powerUp.end(),
                                     [](const auto &pu)
                                     { return pu->getSprite().getPosition().x < -50 || pu->getIsActive(); }),
                      powerUp.end());

        // --- RENDER ---
        window->clear(sf::Color::Black);
        background.render(window.get());
        for (auto &o : obstacles)
            o->render(window.get());
        for (auto &p : powerUp)
            p->render(window.get());
        for (auto &e : enemies)
            e->render(window.get());
        for (auto &c : coins)
            c->render(window.get());
        player->render(window.get());
        scoreDisplay.render(window.get());
        healthBar.render(window.get());

        // DELEGATE TO StatusHUD
        statusHUD.render(window.get(), player->getIsInvincible(), player->getIsRapidFire());

        // --- DRAW DEBUG TEXT ---
        if (debugMode)
        {
            window->draw(debugText);
        }

        window->display();
    }
}

void GameState::handleInput()
{
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            isRunning = false;
            window->close();
        }
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                isRunning = false;
                window->close();
            }

            // --- DEBUG TOGGLE ---
            if (event.key.code == sf::Keyboard::Grave)
            {
                debugMode = !debugMode;
            }

            if (debugMode && gameState == 1)
            {
                if (event.key.code == sf::Keyboard::Num1)
                {
                    coins.push_back(std::make_unique<Coin>());
                }
                if (event.key.code == sf::Keyboard::Num2)
                {
                    obstacles.push_back(std::make_unique<Spike>());
                }
                if (event.key.code == sf::Keyboard::Num3)
                {
                    enemies.push_back(std::make_unique<Walker>(1300.0f, 400.0f));
                }
                if (event.key.code == sf::Keyboard::Num4)
                {
                    enemies.push_back(std::make_unique<Drone>(1300.0f, 300.0f));
                }
                if (event.key.code == sf::Keyboard::Num5)
                {
                    powerUp.push_back(std::make_unique<PowerUp>());
                }
                if (event.key.code == sf::Keyboard::Num0)
                {
                    player->setInvincible(!player->getIsInvincible(), 9999.0f);
                }
            }

            if (gameState == 2)
            {
                if (event.key.code == sf::Keyboard::Enter)
                {
                    reset();
                    gameState = 1;
                    playSound(collectSound);
                }
                if (event.key.code == sf::Keyboard::Left)
                {
                    difficulty--;
                    if (difficulty < 0)
                        difficulty = 2;
                    playSound(collectSound);
                }
                if (event.key.code == sf::Keyboard::Right)
                {
                    difficulty++;
                    if (difficulty > 2)
                        difficulty = 0;
                    playSound(collectSound);
                }
            }
            if (gameState == 0 && event.key.code == sf::Keyboard::Enter)
                gameState = 2;

            if (gameState == 1)
            {
                if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
                {
                    player->jump();
                    playSound(jumpSound);
                }
                if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
                    player->ground();
            }
        }
    }

    if (gameState == 1)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            player->move(-1.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            player->move(1.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            player->shoot();
        }
    }
}

void GameState::checkCollisions()
{
    sf::FloatRect playerBounds = player->getSprite().getGlobalBounds();

    for (auto it = coins.begin(); it != coins.end();)
    {
        if (playerBounds.intersects((*it)->getHitbox()))
        {
            addScore((*it)->getValue());
            playSound(collectSound);
            it = coins.erase(it);
        }
        else
            ++it;
    }

    for (auto &obstacle : obstacles)
    {
        Spike *spike = dynamic_cast<Spike *>(obstacle.get());
        if (spike && !spike->getHasHit())
        {
            if (spike->checkCollision(player.get()))
            {
                player->takeDamage();
                playSound(hitSound);
                if (!player->getIsInvincible())
                    addScore(-25);
                spike->setHasHit(true);
            }
        }
    }

    for (auto &pu : powerUp)
    {
        if (!pu->getIsActive() && playerBounds.intersects(pu->getHitbox()))
        {
            pu->activate();
            playSound(collectSound);
            int type = pu->getType();
            if (type == 0)
                player->setInvincible(true, pu->getDuration());
            else if (type == 1)
                player->setRapidFire(true, pu->getDuration());
            else if (type == 2)
            {
                if (player->getHealth() >= player->getMaxHealth())
                    addScore(50);
                else
                    player->heal(1);
            }
        }
    }

    for (auto &enemy : enemies)
    {
        if (enemy->isAlive() && playerBounds.intersects(enemy->getHitbox()))
        {
            player->takeDamage();
            playSound(hitSound);
        }
        for (auto &proj : player->getProjectiles())
        {
            if (proj->checkCollision(enemy.get()))
            {
                enemy->takeDamage(proj->getDamage());
                proj->getSprite().setPosition(2000, 0);
                addScore(50);
            }
        }
        for (auto &proj : enemy->getProjectiles())
        {
            if (proj->checkCollision(player.get()))
            {
                player->takeDamage();
                playSound(hitSound);
                proj->getSprite().setPosition(-2000, 0);
            }
        }
    }
}

void GameState::reset()
{
    score = 0;
    player = std::make_unique<Player>();
    enemies.clear();
    obstacles.clear();
    coins.clear();
    powerUp.clear();
    coinSpawnTimer = 0.0f;
    spikeSpawnTimer = 0.0f;
    powerUpSpawnTimer = 0.0f;
    enemySpawnTimer = 0.0f;
}