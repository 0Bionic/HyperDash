#include "hyperdash.hpp"
#include "screenObjects.hpp"

#include <cstdlib>
#include <ctime>

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
    Background background("sprites/background.png", 2.0f);
    HealthBar healthBar(1000, 40, 250, 20, 9);
    sf::Clock clock;
    float coinSpawnTimer = 0.0f;

    while (isRunning && window->isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        handleInput();
        update();
        player->updateAnimation(deltaTime);
        background.update();
        healthBar.update(player->getHealth());

        // Coin spawning logic (35% chance)
        coinSpawnTimer += deltaTime;
        if (coinSpawnTimer >= 1.5f)
        {
            coinSpawnTimer = 0.0f;
            if (rand() % 100 < 75)
            {
                coins.push_back(std::make_unique<Coin>());
            }
        }

        // Update coins
        for (auto &coin : coins)
        {
            coin->update();
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

        window->clear(sf::Color::Black);
        background.render(window.get());
        player->render(window.get());

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
            if (event.key.code == sf::Keyboard::Space ||
                event.key.code == sf::Keyboard::W ||
                event.key.code == sf::Keyboard::Up)
            {
                player->jump();
            }
        }
    }

    // Continuous movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        player->move(-1.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        player->move(1.0f);
    }
}

void GameState::update()
{
    player->update();
}

void GameState::checkCollisions()
{
    sf::FloatRect playerBounds = player->getSprite().getGlobalBounds();

    // Check coin collisions
    for (auto it = coins.begin(); it != coins.end();)
    {
        if (playerBounds.intersects((*it)->getSprite().getGlobalBounds()))
        {
            addScore((*it)->getValue());
            it = coins.erase(it);
        }
        else
        {
            ++it;
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