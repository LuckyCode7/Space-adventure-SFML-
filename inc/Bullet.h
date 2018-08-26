#pragma once
#include <SFML/Graphics.hpp>

class Bullet : public sf::CircleShape
{
private:
    int speed;
    bool isExtra;
    sf::RectangleShape extraBullet;
    sf::Texture extraBullet_tx;
public:
    Bullet(const int& XPOS, const int& YPOS, bool extra);
    int getSpeed() const;
    bool getIsExtra() const;
    sf::RectangleShape getExtraBullet();
    void moveBullet(const int& YPOS);
    virtual ~Bullet();
};
