#include "hyperdash.hpp"
#include "screenObjects.hpp"

GameState::GameState()
    : score(0), isRunning(true), gameState(1)
{
    window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(1280, 720),
        "HyperDash");
    window->setFramerateLimit(60);

    player = std::make_unique<Player>();
}

void GameState::run()
{
    Background background("sprites/background.png", 2.0f);
    HealthBar healthBar(1000, 40, 250, 15, 9);
    sf::Clock clock;

    while (isRunning && window->isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        handleInput();
        update();
        player->updateAnimation(deltaTime);
        background.update();
        healthBar.update(player->getHealth());

        window->clear(sf::Color::Black);
        background.render(window.get());
        player->render(window.get());
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
        if (event.key.code == sf::Keyboard::H)
        {
            player->takeDamage(); // temporary
        }
    }

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

void GameState::reset()
{
    score = 0;
    player = std::make_unique<Player>();
    enemies.clear();
    obstacles.clear();
    coins.clear();
}