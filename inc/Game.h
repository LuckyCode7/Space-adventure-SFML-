#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <unordered_map>
#include <sstream>
#include <memory>
#include <string>
#include "Player.h"
#include "Meteorite.h"
#include "extraHealth.h"
#include "SoundsPlayer.h"

class Bullet;
class Player;
class Meteorite;
class ExtraHealth;
class MySound;

class Game : public sf::RenderWindow
{
private:
    sf::Vertex dust[1000];
    sf::Texture background_tx;
    sf::Text text;
    sf::RectangleShape background;
    sf::Clock gameTime;
    sf::Clock extraHealthClock;
    sf::Text tickTack_txt;
    sf::Text points_txt;

    bool isGamePaused = true;
    bool isGameOver = false;
    bool isMapChanged = false;

    int minutes;
    int seconds;
    int rememberSeconds;
    int points;

    std::unordered_map<int, std::string> maps;
    std::vector <Meteorite*> meteorites;
    std::vector <ExtraHealth*> bootlesOfHealth;
    std::stringstream ss;

    void setConsole(const std::string& title, const int& width, const int& height);
    void setGameWindow(HWND hwnd, const int& transparency);
    void setBackground();
    void randomMap();
    void setDust(const int& joystickPosition);
    void setTextProperties(const std::string fontPath);
    void setAllBullets(std::vector<Bullet*>& b, const std::unique_ptr<Player>& player);
    void removeComponents(std::vector<Bullet*>& b);
    void updateTickTack();
    void createMeteorite();
    void setAllMeteorites(bool isGamePaused);
    void updatePointsText();
    void checkBulletMeteoriteCollision(std::vector<Bullet*>& b, const std::unique_ptr<Player>& player);
    void checkPlayerMeteoriteCollision(const std::unique_ptr<Player>& player);
    void createExtraHealth(const std::unique_ptr<Player>& player);
    void updateExtraHealth(const std::unique_ptr<Player>& player);
    bool gameOver(const std::unique_ptr<Player>& player);
    void removeSounds();
    void automaticMapChange(const std::unique_ptr<Player>& player);
    void saveResults();
public:
    static sf::Font font;
    static std::vector<MySound*> sounds;
    Game(const sf::String&);
    void startGame();
    virtual ~Game();
};