#include "hyperdash.hpp"
#include "background.hpp"

GameState::GameState()
    : score(0), isRunning(true), gameState(1)
{
    window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(1280, 720),
        "HyperDash");
    window->setFramerateLimit(60);
}

void GameState::run()
{
    Background background("sprites/background.png", 2.0f);

    while (isRunning && window->isOpen())
    {
        handleInput();
        background.update();

        window->clear(sf::Color::Black);
        background.render(window.get());
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
        }
    }
}