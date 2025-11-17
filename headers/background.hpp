#ifndef BACKGROUND_H
#define BACKGROUND_H

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

#endif