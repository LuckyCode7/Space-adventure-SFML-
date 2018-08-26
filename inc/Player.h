#pragma once
#include <SFML/Graphics.hpp>
#include <sstream>
#include <vector>
#include "Bullet.h"

class Game;
class MySound;
class Player : public sf::Sprite
{
private:
    sf::Image ship_img;
    sf::Texture ship_tx;
    sf::Texture hp_tx;
    sf::Texture extraBullet_tx;
    sf::RectangleShape hpSign;
    sf::RectangleShape hpRect;
    sf::RectangleShape extraBulletSign;
    sf::RectangleShape extraBulletRect;
    sf::Vertex power[100];
    sf::Text points_txt;
    sf::Text bullets_txt;
    sf::Clock leftShot;
    sf::Clock rightShot;
    sf::Clock extraShot;

    std::stringstream ss;
    int bullets = 50;
    int hp = 200;

    void checkPosition();
    void setShipPosition();
    void setPower();
    void setHealth();
    void setBulletsText();
    void updateBulletsText();
    void setExtraBullet();
    void updateExtraBulletRect();
    void updateHpRect();
public:
    static std::vector<Bullet*> fire;
    static std::vector<sf::Vector2i> pointsAroundShip;

    Player();
    void drawShip(Game& game, bool isGamePaused);
    void setNumberofBullets(const int& howManyBullets);
    void detectGreenPoints();
    void setHp(const int& hp);
    int getHp() const;
    virtual ~Player();
};

