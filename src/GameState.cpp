#include "hyperdash.hpp"
#include "screenObjects.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>

GameState::GameState()
    : score(0), isRunning(true), gameState(1)
{
    window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(1280, 720),
        "HyperDash");
    window->setFramerateLimit(60);

    player = std::make_unique<Player>();

    srand(static_cast<unsigned>(time(0)));
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
    float coinSpawnTimer = 0.0f;
    float spikeSpawnTimer = 0.0f;
    float scoreIncreaseTimer = 0.0f;

    while (isRunning && window->isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        float elapsedTime = gameTimer.getElapsedTime().asSeconds();

        handleInput();

        // Check if player is dead
        if (player->getHealth() <= 0)
        {
            gameState = 0;
        }

        if (gameState == 0)
        {
            // Game Over state
            gameOverScreen.update(deltaTime);

            window->clear(sf::Color::Black);
            gameOverScreen.render(window.get());
            window->display();
            continue;
        }

        // Calculate current speed (increases gradually over time)
        float currentSpeed = baseScrollSpeed * (1.0f + (elapsedTime / 60.0f));

        // Normal gameplay (gameState == 1)
        player->updateAnimation(deltaTime);
        player->update(deltaTime);
        background.setSpeed(currentSpeed);
        background.update();
        healthBar.update(player->getHealth());
        scoreDisplay.update(score);

        // Coin spawning logic (45% chance)
        coinSpawnTimer += deltaTime;
        if (coinSpawnTimer >= 1.5f)
        {
            coinSpawnTimer = 0.0f;
            if (rand() % 100 < 45)
            {
                auto coin = std::make_unique<Coin>();
                coin->setScrollSpeed(currentSpeed);
                coins.push_back(std::move(coin));
            }
        }

        // Spike spawning logic (40% chance)
        spikeSpawnTimer += deltaTime;
        if (spikeSpawnTimer >= 2.0f)
        {
            spikeSpawnTimer = 0.0f;
            if (rand() % 100 < 40)
            {
                auto spike = std::make_unique<Spike>();
                spike->setScrollSpeed(currentSpeed);
                obstacles.push_back(std::move(spike));
            }
        }

        scoreIncreaseTimer += deltaTime;
        if (scoreIncreaseTimer >= 0.75f)
        {
            scoreIncreaseTimer = 0.0f;
            if (score % 250 == 0 && score != 0)
            {
                addScore(25);
            }
            else
            {
                addScore(10);
            }
        }

        // Update existing coins and obstacles with current speed
        for (auto &coin : coins)
        {
            coin->setScrollSpeed(currentSpeed);
            coin->update();
        }

        for (auto &obstacle : obstacles)
        {
            Spike *spike = dynamic_cast<Spike *>(obstacle.get());
            if (spike)
            {
                spike->setScrollSpeed(currentSpeed);
            }
            obstacle->update();
        }

        checkCollisions();

        // Remove coins that went off screen
        coins.erase(
            std::remove_if(coins.begin(), coins.end(),
                           [](const std::unique_ptr<Coin> &coin)
                           {
                               return coin->getSprite().getPosition().x < -50;
                           }),
            coins.end());

        // Remove obstacles that went off screen
        obstacles.erase(
            std::remove_if(obstacles.begin(), obstacles.end(),
                           [](const std::unique_ptr<Obstacle> &obstacle)
                           {
                               return obstacle->getSprite().getPosition().x < -50;
                           }),
            obstacles.end());

        window->clear(sf::Color::Black);
        background.render(window.get());

        // Render obstacles (spikes)
        for (auto &obstacle : obstacles)
        {
            obstacle->render(window.get());
        }

        player->render(window.get());
        scoreDisplay.render(window.get());

        for (auto &coin : coins)
        {
            coin->render(window.get());
        }

        healthBar.render(window.get());
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

            // Restart on Enter when game over
            if (event.key.code == sf::Keyboard::Enter && gameState == 0)
            {
                reset();
                gameState = 1;
                return;
            }

            // Gameplay controls (only when gameState == 1)
            if (gameState == 1)
            {
                if (event.key.code == sf::Keyboard::W ||
                    event.key.code == sf::Keyboard::Up)
                {
                    player->jump();
                }
                if (event.key.code == sf::Keyboard::H)
                {
                    player->takeDamage();
                    addScore(-25);
                }
                if (event.key.code == sf::Keyboard::Down ||
                    event.key.code == sf::Keyboard::S)
                {
                    player->ground();
                }
            }
        }
    }

    // Movement controls (only when gameState == 1)
    if (gameState == 1)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            player->move(-1.25f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            player->move(1.0f);
        }
    }
}

void GameState::checkCollisions()
{
    sf::FloatRect playerBounds = player->getSprite().getGlobalBounds();

    // Check coin collisions
    for (auto it = coins.begin(); it != coins.end();)
    {
        if (playerBounds.intersects((*it)->getHitbox()))
        {
            addScore((*it)->getValue());
            it = coins.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Check spike collisions (only damage once per spike)
    for (auto &obstacle : obstacles)
    {
        Spike *spike = dynamic_cast<Spike *>(obstacle.get());
        if (spike && !spike->getHasHit())
        {
            if (spike->checkCollision(player.get()))
            {
                player->takeDamage();
                addScore(-25);
                spike->setHasHit(true);
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
}