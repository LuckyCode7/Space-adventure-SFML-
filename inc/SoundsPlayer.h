#pragma once
#include <SFML/Audio.hpp>
#include <string>

class MySound : public sf::Sound
{
private:
    sf::SoundBuffer buffer;
    std::string soundPath;
    int volume;
public:
    MySound(const std::string& soundPath_, const int& volume_);
    ~MySound();
};