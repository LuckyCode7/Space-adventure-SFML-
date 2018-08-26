#include "Game.h"
#include "Define.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <math.h>

sf::Font Game::font;
std::vector<MySound*> Game::sounds;

Game::Game(const sf::String& title) :
    sf::RenderWindow(sf::VideoMode(SCREENX, SCREENY, CBITS), title)
    , minutes(0)
    , seconds(0)
    , rememberSeconds(0)
    , points(0)
{
    this->setConsole(title, 400, 450);
    this->setGameWindow(getSystemHandle(), 100);
    this->setActive(true);
    this->setFramerateLimit(30);
    this->setPosition(sf::Vector2i(POSX, POSY));
    this->setKeyRepeatEnabled(true);
    this->setMouseCursorVisible(false);

    this->setBackground();
    this->setTextProperties("fonts/OpenSans-Light.ttf");

    for (int i = 0; i < 1000; i++)
    {
        dust[i].position = sf::Vector2f(rand() % SCREENX, rand() % SCREENY);
        dust[i].color = sf::Color(100 + rand() % 155, 100 + rand() % 155, 100 + rand() % 155, 255);
    }
}

void Game::setTextProperties(const std::string fontPath)
{
    this->font.loadFromFile(fontPath);
    this->text.setFont(font);
    this->text.setPosition(SCREENX / 4, SCREENY - 70);
    this->text.setCharacterSize(30);
    this->text.setFillColor(sf::Color(234, 237, 235, 130));
    this->text.setStyle(sf::Text::Bold);
    this->text.setOutlineColor(sf::Color(0, 0, 0, 255));
    this->text.setOutlineThickness(3);

    this->tickTack_txt.setFont(font);
    this->tickTack_txt.setPosition(SCREENX - 200, 15);
    this->tickTack_txt.setCharacterSize(30);
    this->tickTack_txt.setFillColor(sf::Color(234, 237, 235, 130));
    this->tickTack_txt.setStyle(sf::Text::Bold);
    this->tickTack_txt.setOutlineColor(sf::Color(0, 0, 0, 255));
    this->tickTack_txt.setOutlineThickness(3);

    this->points_txt.setFont(font);
    this->points_txt.setPosition(SCREENX - 200, SCREENY - 50);
    this->points_txt.setCharacterSize(30);
    this->points_txt.setFillColor(sf::Color(204, 155, 10, 130));
    this->points_txt.setStyle(sf::Text::Bold);
    this->points_txt.setOutlineColor(sf::Color(0, 0, 0, 255));
    this->points_txt.setOutlineThickness(3);
}

void Game::setAllBullets(std::vector<Bullet*>& b, const std::unique_ptr<Player>& player)
{
    for (auto x = b.begin(); x != b.end(); x++)
    {
        (*x)->moveBullet(player->getPosition().y);
        if ((*x)->getIsExtra() == false)
            this->draw(**x);
        else
            this->draw((*x)->getExtraBullet());
    }
}

void Game::removeComponents(std::vector<Bullet*>& b)
{
    for (auto x = b.begin(); x != b.end(); x++)
    {
        if ((*x)->getPosition().x > SCREENX + 40 || (*x)->getExtraBullet().getPosition().x > SCREENX + 40)
        {
            delete *x;
            b.erase(x);
            break;
        }
    }

    for (auto x = meteorites.begin(); x != meteorites.end(); x++)
    {
        if ((*x)->getPosition().x < POSX - 50)
        {
            delete *x;
            meteorites.erase(x);
            break;
        }
    }
}

void Game::updateTickTack()
{
    seconds = (int)gameTime.getElapsedTime().asSeconds() + rememberSeconds;

    if (seconds >= 60)
    {
        this->seconds = 0;
        this->rememberSeconds = 0;
        this->gameTime.restart();
        this->minutes++;
    }
    this->ss.str("");
    this->ss << "Time: " << minutes << ":" << seconds;
    this->tickTack_txt.setString(this->ss.str());
    this->draw(tickTack_txt);
}

void Game::createMeteorite()
{
    if (minutes >= 0 && minutes <= 3 && seconds > 5)
    {
        if (rand() % 20 == 1)
        {
            Meteorite* met = new Meteorite(rand() % 10 + 1, rand() % 5 + 5);
            this->meteorites.emplace_back(met);
        }
    }

    if (minutes > 3 && minutes <= 6)
    {
        if (rand() % 20 == 1 || rand() % 20 == 2)
        {
            Meteorite* met = new Meteorite(rand() % 20 + 1, rand() % 5 + 5);
            this->meteorites.emplace_back(met);
        }
    }

    if (minutes > 6)
    {
        if (rand() % 20 == 1 || rand() % 20 == 2)
        {
            Meteorite* met = new Meteorite(rand() % seconds + 1, rand() % 5 + 5);
            this->meteorites.emplace_back(met);
        }
    }
}

void Game::setAllMeteorites(bool isGamePaused)
{
    for (auto x = meteorites.begin(); x != meteorites.end(); x++)
    {
        if (isGamePaused == false)
            (*x)->moveMeteorite();
        this->draw(**x);
    }
}

void Game::updatePointsText()
{
    this->ss.str("");
    this->ss << "Points: " << points;
    this->points_txt.setString(this->ss.str());
    this->draw(points_txt);
}

void Game::checkBulletMeteoriteCollision(std::vector<Bullet*>& b, const std::unique_ptr<Player>& player)
{
    for (auto bullet = b.begin(); bullet != b.end(); bullet++)
    {
        for (auto meteorite = meteorites.begin(); meteorite != meteorites.end(); meteorite++)
        {
            if ((*bullet)->getGlobalBounds().intersects((*meteorite)->getGlobalBounds())
                || (*bullet)->getExtraBullet().getGlobalBounds().intersects((*meteorite)->getGlobalBounds()))
            {
                if ((*bullet)->getIsExtra())
                {
                    MySound* sound = new MySound("sounds/extracrash2.wav", abs(100 - (*meteorite)->getPosition().x));
                    this->sounds.emplace_back(sound);
                    sound->play();
                    this->points += (*meteorite)->getSpeed() + 10;
                    delete *bullet;
                    b.erase(bullet);
                    delete *meteorite;
                    this->meteorites.erase(meteorite);
                    player->setNumberofBullets(3);
                }
                else
                {
                    MySound* sound = new MySound("sounds/crash.wav", abs(100 - (*meteorite)->getPosition().x));
                    this->sounds.emplace_back(sound);
                    sound->play();
                    player->setNumberofBullets(1);
                    (*meteorite)->setPointCount((*meteorite)->getPointCount() - 1);
                    (*meteorite)->setSpeed(2);
                    delete *bullet;
                    b.erase(bullet);
                    if ((*meteorite)->getPointAcces()) this->points += (*meteorite)->getSpeed();
                    (*meteorite)->setTrajecory(rand() % 2 - 1);
                    if ((*meteorite)->meteoriteHit())
                    {
                        delete *meteorite;
                        this->meteorites.erase(meteorite);
                        this->points += 10;
                    }
                }
                return;
            }
        }
    }
}

void Game::checkPlayerMeteoriteCollision(const std::unique_ptr<Player>& player)
{
    for (auto meteorite = meteorites.begin(); meteorite != meteorites.end(); meteorite++)
    {
        for (auto point = Player::pointsAroundShip.begin(); point != Player::pointsAroundShip.end(); point++)
        {
            if ((*meteorite)->getGlobalBounds().intersects(player->getGlobalBounds()) &&
                ((*meteorite)->getGlobalBounds().contains(player->getPosition().x - ((point->x) / 2), player->getPosition().y - ((point->y) / 2))))
            {
                MySound* sound = new MySound("sounds/extracrash.wav", 50);
                this->sounds.emplace_back(sound);
                sound->play();
                delete *meteorite;
                this->meteorites.erase(meteorite);
                player->setHp(-20);
                return;
            }
        }
    }
}

void Game::createExtraHealth(const std::unique_ptr<Player>& player)
{
    if (player->getHp() <= 60)
    {
        if (extraHealthClock.getElapsedTime().asSeconds() >= 4)
        {
            ExtraHealth* extrahp = new ExtraHealth();
            this->bootlesOfHealth.emplace_back(extrahp);
            extraHealthClock.restart();
        }
    }
}

void Game::updateExtraHealth(const std::unique_ptr<Player>& player)
{
    for (auto hp = bootlesOfHealth.begin(); hp != bootlesOfHealth.end(); hp++)
    {
        this->draw(**hp);
        (*hp)->moveExtraHealth();
        if ((*hp)->getPosition().y > SCREENY + 50)
        {
            delete (*hp);
            this->bootlesOfHealth.erase(hp);
            return;
        }
        if (player->getGlobalBounds().intersects((*hp)->getGlobalBounds()))
        {
            MySound* sound = new MySound("sounds/extrahp.wav", 50);
            this->sounds.emplace_back(sound);
            sound->play();
            player->setHp(20);
            delete (*hp);
            this->bootlesOfHealth.erase(hp);
            return;
        }
    }
}

bool Game::gameOver(const std::unique_ptr<Player>& player)
{
    if (player->getHp() > 0)
    {
        this->removeSounds();
        this->setDust(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X));
        this->automaticMapChange(player);
        this->removeComponents(Player::fire);
        this->createMeteorite();
        this->createExtraHealth(player);
        this->checkBulletMeteoriteCollision(Player::fire, player);
        this->checkPlayerMeteoriteCollision(player);
        sf::sleep(sf::milliseconds(10));
        this->clear(sf::Color(0xffffffff));
        this->draw(this->background);
        this->draw(dust, 1000, sf::PrimitiveType::Points);
        this->setAllBullets(Player::fire, player);
        this->setAllMeteorites(false);
        this->updateExtraHealth(player);
        this->updateTickTack();
        this->updatePointsText();
        player->drawShip(*this, false);
        this->display();
        return false;
    }
    else
    {
        this->isGameOver = true;
        this->gameTime.restart();
        this->rememberSeconds = seconds;
        this->text.setString("Game over -> press [esc] to close the game");
        this->removeComponents(Player::fire);
        sf::sleep(sf::milliseconds(60));
        this->clear(sf::Color(0xffffffff));
        this->draw(this->background);
        this->setAllMeteorites(true);
        this->draw(text);
        this->draw(tickTack_txt);
        this->updatePointsText();
        this->display();
        return true;
    }
}

void Game::removeSounds()
{
    if (sounds.size() > 4)
    {
        delete *(sounds.begin());
        sounds.erase(sounds.begin());
    }
}

void Game::automaticMapChange(const std::unique_ptr<Player>& player)
{
    if (minutes % 2 == 0 && seconds == 0 && !isMapChanged)
    {
        isMapChanged = true;
        this->setBackground();
    }
    if (minutes % 2 != 0)
        isMapChanged = false;
}

void Game::saveResults()
{
    std::ofstream outFile("Results/Results.txt", std::ios_base::out | std::ios_base::ate | std::ios_base::app);
    outFile << "Date: " << __TIMESTAMP__ << std::endl;
    outFile << "Time in the game: " << minutes << ":" << seconds << std::endl;
    outFile << "Points: " << points << "\n\n";
    outFile.close();
}

void Game::setConsole(const std::string& title, const int& width, const int& height)
{
    std::wstring temp = std::wstring(title.begin(), title.end());
    LPCWSTR sw = temp.c_str();
    HWND cw = GetConsoleWindow();
    SetWindowText(cw, sw);
    SetWindowPos(cw, HWND_TOPMOST, 0, 0, width, height, SWP_NOMOVE);
}

void Game::setGameWindow(HWND hwnd, const int& transparency)
{
    RECT rect;
    GetWindowRect(hwnd, &rect);
    SetWindowPos(hwnd, HWND_TOPMOST, rect.left, rect.top, rect.right, rect.bottom, SWP_NOMOVE);
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, 0, (255 * transparency) / 100, LWA_ALPHA);
}

void Game::setBackground()
{
    randomMap();
    this->background_tx.loadFromFile(maps.find(rand() % 4)->second);
    this->background.setTexture(&(this->background_tx));
    this->background.setPosition(0, 0);
    this->background.setSize(sf::Vector2f(SCREENX, SCREENY));
}

void Game::randomMap()
{
    std::pair<int, std::string> pair_;
    while (maps.size() != 4)
    {
        pair_.first = rand() % 4;
        switch (pair_.first)
        {
        case 0:
            pair_.second = "images/background/bg1.png";
            break;
        case 1:
            pair_.second = "images/background/bg2.png";
            break;
        case 2:
            pair_.second = "images/background/bg3.png";
            break;
        case 3:
            pair_.second = "images/background/bg4.png";
            break;
        }
        this->maps.insert(pair_);
    }
}

void Game::setDust(const int& joystickPosition)
{
    for (int i = 0; i < 1000; i++)
    {
        if (joystickPosition > 50)
        {
            dust[i].position.x -= (i % 3 + 20);
        }
        else
        {
            dust[i].position.x -= (i % 3 + 1);
        }
        if (dust[i].position.x <= 0)
        {
            Game::dust[i].position = sf::Vector2f(SCREENX, dust[i].position.y);
        }
    }
}

void Game::startGame()
{
    std::unique_ptr<Player> player(new Player);
    sf::Event event;
    while (this->isOpen())
    {
        while (this->pollEvent(event))
        {
            if (event.type == sf::Event::Closed || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
            {
                this->saveResults();
                this->close();
            }
            if (sf::Joystick::isButtonPressed(0, 0)) isGamePaused = !isGamePaused;
        }

        if (isGamePaused && !isGameOver)
        {
            gameTime.restart();
            this->rememberSeconds = seconds;
            this->clear(sf::Color(0xffffffff));
            this->text.setString("Game is paused -> click [X] to continue");
            this->draw(this->background);
            this->draw(this->dust, 1000, sf::PrimitiveType::Points);
            this->setAllMeteorites(true);
            player->drawShip(*this, true);
            this->draw(this->text);
            this->draw(tickTack_txt);
            this->updatePointsText();
            sf::sleep(sf::milliseconds(50));
            this->display();
            continue;
        }
        this->gameOver(player);
    }
}

Game::~Game() {}