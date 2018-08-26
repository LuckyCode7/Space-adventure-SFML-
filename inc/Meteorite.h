#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Meteorite : public sf::ConvexShape
{
private:
    int speed;
    int vertices;
    int hit;
    bool pointsAcces;
    sf::Texture meteorite_tx;
    sf::Transform transform;
    std::vector <sf::Vector2f> vertex;
    int trajectory;
    void setMeteoriteTexture(const int& choice);
    void setAllVertices();
public:
    Meteorite(const int& speed_, const int& vertices_);
    ~Meteorite();
    void moveMeteorite();
    int getSpeed() const;
    void setSpeed(const int& speed);
    void setTrajecory(const int& trajectory);
    bool meteoriteHit();
    bool getPointAcces() const;
};
