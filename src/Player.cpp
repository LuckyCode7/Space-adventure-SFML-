#include "Player.h"
#include "Define.h"
#include "Game.h"
#include "SoundsPlayer.h"

std::vector<Bullet*> Player::fire;
std::vector<sf::Vector2i> Player::pointsAroundShip;

Player::Player() : Sprite()
{
    this->ship_img.loadFromFile("images/ship2.png");
    this->detectGreenPoints();
    this->ship_tx.loadFromImage(ship_img);
    this->setTexture(ship_tx);
    this->setOrigin(ship_tx.getSize().x / 2, ship_tx.getSize().y / 2);
    this->setPosition(ship_tx.getSize().x, SCREENY / 2);
    this->setTextureRect(sf::IntRect(0, 0, ship_tx.getSize().x, ship_tx.getSize().y));
    this->scale(0.5, 0.5);
    this->setHealth();
    this->setBulletsText();
    this->updateBulletsText();
    this->setExtraBullet();
}

void Player::drawShip(Game& game, bool isGamePaused)
{
    if (!isGamePaused)
    {
        setPower();
        this->checkPosition();
        this->setShipPosition();
    }

    if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) > 0)
        game.draw(this->power, 100, sf::PrimitiveType::Points);
    this->updateExtraBulletRect();
    this->updateHpRect();
    game.draw(*this);
    game.draw(hpSign);
    game.draw(hpRect);
    game.draw(bullets_txt);
    game.draw(extraBulletSign);
    game.draw(extraBulletRect);
}

void Player::setNumberofBullets(const int& howManyBullets)
{
    this->bullets += howManyBullets;
    if (this->bullets > 50) this->bullets = 50;
}

void Player::detectGreenPoints()
{
    for (unsigned int x = 0; x < ship_img.getSize().x; x++)
    {
        for (unsigned int y = 0; y < ship_img.getSize().y; y++)
        {
            if (ship_img.getPixel(x, y) == sf::Color(0, 0, 0, 255))
            {
                this->pointsAroundShip.emplace_back(sf::Vector2i(x, y));
                this->ship_img.setPixel(x, y, sf::Color(0, 0, 0, 0));
            }
        }
    }
}

void Player::setHp(const int& hp)
{
    this->hp += hp;
    if (this->hp < 0) this->hp = 0;
    if (this->hp > 200) this->hp = 200;
}

int Player::getHp() const
{
    return this->hp;
}

void Player::checkPosition()
{
    if (this->getPosition().x < POSX + 25)
        this->setPosition(POSX + 25, this->getPosition().y);
    if (this->getPosition().x > SCREENX)
        this->setPosition(POSX + 25, this->getPosition().y);

    if (this->getPosition().y < POSY + 25)
        this->setPosition(this->getPosition().x, POSY + 25);
    if (this->getPosition().y > SCREENY - 43)
        this->setPosition(this->getPosition().x, SCREENY - 43);
}

void Player::setShipPosition()
{
    updateBulletsText();

    this->setPosition(this->getPosition().x + (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) / 10),
        this->getPosition().y + (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) / 10));

    if (sf::Joystick::isButtonPressed(0, 4) && bullets > 0)
    {
        if (this->leftShot.getElapsedTime().asSeconds() >= 0.16)
        {
            MySound* sound = new MySound("sounds/shoot.wav", 40);
            Game::sounds.emplace_back(sound);
            sound->play();
            this->bullets--;
            Bullet* bullet = new Bullet(this->getPosition().x + 34, this->getPosition().y - 22, false);
            this->fire.emplace_back(bullet);
            this->leftShot.restart();
        }
    }

    if (sf::Joystick::isButtonPressed(0, 5) && bullets > 0) 
    {
        if (this->rightShot.getElapsedTime().asSeconds() >= 0.16)
        {
            MySound* sound = new MySound("sounds/shoot.wav", 40);
            Game::sounds.emplace_back(sound);
            sound->play();
            this->bullets--;
            Bullet* bullet = new Bullet(this->getPosition().x + 34, this->getPosition().y + 22, false);
            this->fire.emplace_back(bullet);
            this->rightShot.restart();
        }
    }

    if (sf::Joystick::isButtonPressed(0, 1) && bullets >= 5) 
    {
        if (this->extraShot.getElapsedTime().asSeconds() >= 5.0)
        {
            MySound* sound = new MySound("sounds/shoot2.wav", 40);
            Game::sounds.emplace_back(sound);
            sound->play();
            this->bullets -= 2;
            this->extraBulletRect.setSize(sf::Vector2f(0, extraBulletRect.getSize().y));
            Bullet* bullet = new Bullet(this->getPosition().x, this->getPosition().y, true);
            this->fire.emplace_back(bullet);
            this->extraShot.restart();
        }
    }
}

void Player::setPower()
{
    for (int i = 0; i < 50; i++)
    {
        this->power[i].position.x = this->getPosition().x - 60 - (rand() % 31 - 50) - 2 * i;
        this->power[i].position.y = (rand() % (i + 5) - i / 2) + this->getPosition().y;
        this->power[i].color = sf::Color(255, 0, 0, 255);
    }
}

void Player::setHealth()
{
    this->hp_tx.loadFromFile("images/hp4.png");
    this->hpSign.setTexture(&hp_tx);
    this->hpSign.setSize(sf::Vector2f(30, 30));
    this->hpSign.setPosition(10, 10);

    this->hpRect.setSize(sf::Vector2f(hp, 20));
    this->hpRect.setPosition(hpSign.getPosition().x + 40, 18);
    this->hpRect.setFillColor(sf::Color(255, 0, 0, 130));
    this->hpRect.setOutlineColor(sf::Color(0, 0, 0, 255));
    this->hpRect.setOutlineThickness(3);
}

void Player::setExtraBullet()
{
    this->extraBullet_tx.loadFromFile("images/bsign.png");
    this->extraBulletSign.setTexture(&extraBullet_tx);
    this->extraBulletSign.setSize(sf::Vector2f(19, 30));
    this->extraBulletSign.setPosition(hpSign.getPosition().x, SCREENY - 40);

    this->extraBulletRect.setSize(sf::Vector2f(200, 20));
    this->extraBulletRect.setPosition(hpRect.getPosition().x - 7, extraBulletSign.getPosition().y + 10);
    this->extraBulletRect.setFillColor(sf::Color(0, 0, 255, 130));
    this->extraBulletRect.setOutlineColor(sf::Color(0, 0, 0, 255));
    this->extraBulletRect.setOutlineThickness(3);
}

void Player::updateExtraBulletRect()
{
    int XPOS = extraShot.getElapsedTime().asSeconds() * 40;
    if (extraBulletRect.getSize().x <= 200)
        this->extraBulletRect.setSize(sf::Vector2f(XPOS, 20));
}

void Player::updateHpRect()
{
    this->hpRect.setSize(sf::Vector2f(hp, 20));
}

void Player::setBulletsText()
{
    this->bullets_txt.setFont(Game::font);
    this->bullets_txt.setPosition(51, 45);
    this->bullets_txt.setCharacterSize(25);
    this->bullets_txt.setFillColor(sf::Color(234, 237, 235, 130));
    this->bullets_txt.setStyle(sf::Text::Bold);
    this->bullets_txt.setOutlineColor(sf::Color(0, 0, 0, 255));
    this->bullets_txt.setOutlineThickness(3);
}

void Player::updateBulletsText()
{
    this->ss.str("");
    this->ss << "Bullets: " << this->bullets << " / 50";
    this->bullets_txt.setString(this->ss.str());
}

Player::~Player() {}



