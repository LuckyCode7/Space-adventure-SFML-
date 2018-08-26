#include "extraHealth.h"
#include "Define.h"

ExtraHealth::ExtraHealth() :
    sf::RectangleShape(),
    speed(rand() % 6 + 1)
{
    this->setSize(sf::Vector2f(36, 48));
    this->setPosition(rand() % SCREENX, POSY - 50);
    this->health_tx.loadFromFile("images/hp.png");
    this->setTexture(&health_tx);
    this->setOrigin(this->getPosition().x / 2, this->getPosition().y / 2);
    this->scale(0.7, 0.7);
}

void ExtraHealth::moveExtraHealth()
{
    this->move(0, speed);
    this->setRotation(2);
}

ExtraHealth::~ExtraHealth() {}

