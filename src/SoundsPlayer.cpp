#include "SoundsPlayer.h"
#include <SFML/Audio.hpp>
#include "Define.h"

MySound::MySound(const std::string& soundPath_, const int& volume_) :
    Sound(),
    soundPath(soundPath_),
    volume(volume_)
{
    this->buffer.loadFromFile(soundPath);
    this->setBuffer(buffer);
    this->setVolume(volume);
    this->setAttenuation(1);
    this->setPitch(1);
}

MySound::~MySound() {}

