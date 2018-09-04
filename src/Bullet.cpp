#include <../inc/Bullet.h>

Bullet::Bullet(const int& XPOS, const int& YPOS, bool extra) :
    sf::CircleShape(),
    speed(16),
    isExtra(extra)
{
    if (extra)
    {
        this->extraBullet.setSize(sf::Vector2f(40, 19));
        this->extraBullet_tx.loadFromFile("../images/extra.png");
        this->extraBullet.setTexture(&extraBullet_tx);
        this->extraBullet.setPosition(XPOS, YPOS + 30);
    }
    else
    {
        this->setRadius(3);
        this->setPosition(XPOS + rand() % 10, YPOS + rand() % 20 - 10);
        this->setFillColor(sf::Color(100 + rand() % 155, 100 + rand() % 155, 100 + rand() % 155, 120));
        this->setOutlineColor(sf::Color(0, 0, 0, 255));
        this->setOutlineThickness(3);
    }
}

int Bullet::getSpeed() const
{
    return this->speed;
}

bool Bullet::getIsExtra() const
{
    return this->isExtra;
}

sf::RectangleShape Bullet::getExtraBullet()
{
    return this->extraBullet;
}

void Bullet::moveBullet(const int& YPOS)
{
    int distance = this->extraBullet.getPosition().y - YPOS;
    if (isExtra)
    {
        if (distance > 70)
            this->extraBullet.move(this->speed, 0);
        else
            this->extraBullet.move(this->speed, 5);
    }
    else
        this->move(this->speed, 0);
}

Bullet::~Bullet() {}

