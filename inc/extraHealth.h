#pragma once
#include <SFML/Graphics.hpp>

class ExtraHealth : public sf::RectangleShape
{
private:
    sf::Texture health_tx;
    int speed;
public:
    ExtraHealth();
    void moveExtraHealth();
    ~ExtraHealth();
};
