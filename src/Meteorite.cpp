#include "Meteorite.h"
#include "Define.h"

Meteorite::Meteorite(const int & speed_, const int & vertices_) :
    speed(speed_),
    vertices(vertices_),
    hit(0),
    pointsAcces(true)
{
    this->trajectory = rand() & 2 - 1;
    this->setPointCount(vertices);
    float angle = 360.f / vertices;
    this->setMeteoriteTexture(rand() % 4);
    this->setTexture(&meteorite_tx);
    this->setPosition(sf::Vector2f(sf::Vector2f(SCREENX + 150, rand() % (SCREENY - 50) + 25)));
    this->transform = sf::Transform::Identity;
    this->transform.rotate(angle, this->getPosition().x, this->getPosition().y);
    this->setPoint(0, sf::Vector2f(this->getPosition().x, this->getPosition().y + 100 / (1 + this->speed)));
    this->vertex.push_back(this->getPoint(0));
    this->setAllVertices();
    this->setOrigin(this->getPosition().x, this->getPosition().y);
    this->setOutlineThickness(1);
    this->setOutlineColor(sf::Color(0, 0, 0, 255));
}

void Meteorite::setMeteoriteTexture(const int& choice)
{
    switch (choice)
    {
    case 0: this->meteorite_tx.loadFromFile("images/texture/m1.png"); break;
    case 1: this->meteorite_tx.loadFromFile("images/texture/m2.png"); break;
    case 2: this->meteorite_tx.loadFromFile("images/texture/m3.png"); break;
    case 3: this->meteorite_tx.loadFromFile("images/texture/m4.png"); break;
    }
}

void Meteorite::setAllVertices()
{
    for (int i = 1; i < vertices; i++)
    {
        this->setPoint(i, transform.transformPoint(this->getPoint(i - 1)));
        this->vertex.push_back(this->getPoint(i));
    }
}

void Meteorite::moveMeteorite()
{
    this->move(-speed, trajectory);
    this->rotate(-(speed + 2));
}

int Meteorite::getSpeed() const
{
    return this->speed;
}

void Meteorite::setSpeed(const int& speed)
{
    this->speed += speed;
}

void Meteorite::setTrajecory(const int& trajectory)
{
    this->trajectory += trajectory;
}

bool Meteorite::meteoriteHit()
{
    this->pointsAcces = false;
    this->hit++;
    if (this->hit == 4) return true;
    else return false;
}

bool Meteorite::getPointAcces() const
{
    return this->pointsAcces;
}

Meteorite::~Meteorite() {}
