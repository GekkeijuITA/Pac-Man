#include "../../includes/lib/SoundManager.hpp"
#include <iostream>
#include <cmath>
#include <chrono>

SoundManager::SoundManager()
{
    loadAll();
}

void SoundManager::loadAll()
{
    loadSound("start", "../../Tappa_14/resources/sounds/arcade/start.wav");
    loadSound("credit", "../../Tappa_14/resources/sounds/arcade/credit.wav");

    loadSound("waka", "../../Tappa_14/resources/sounds/waka_mono.ogg");
    loadSound("eat_ghost", "../../Tappa_14/resources/sounds/arcade/eat_ghost.wav");
    loadSound("eat_fruit", "../../Tappa_14/resources/sounds/arcade/eat_fruit.wav");
    loadSound("death", "../../Tappa_14/resources/sounds/arcade/death_0.wav");
    loadSound("death_pop", "../../Tappa_14/resources/sounds/arcade/death_1.wav");
    loadSound("extend", "../../Tappa_14/resources/sounds/arcade/extend.wav");

    // GHOST
    loadSound("ghost_normal0", "../../Tappa_14/resources/sounds/arcade/siren0.wav");
    loadSound("ghost_normal1", "../../Tappa_14/resources/sounds/arcade/siren1.wav");
    loadSound("ghost_normal2", "../../Tappa_14/resources/sounds/arcade/siren2.wav");
    loadSound("ghost_normal3", "../../Tappa_14/resources/sounds/arcade/siren3.wav");
    loadSound("ghost_normal4", "../../Tappa_14/resources/sounds/arcade/siren4.wav");
    loadSound("ghost_scared", "../../Tappa_14/resources/sounds/arcade/fright.wav");
    loadSound("ghost_eaten", "../../Tappa_14/resources/sounds/arcade/eyes.wav");
}

void SoundManager::loadSound(const std::string &name, const std::string &filePath)
{
    sf::SoundBuffer audioBuffer;
    if (!audioBuffer.loadFromFile(filePath))
    {
        std::cerr << "Error loading sound: " << filePath << std::endl;
        return;
    }

    soundEffects[name] = new Audio(audioBuffer);
}

void SoundManager::playSound(const std::string &name)
{
    auto it = soundEffects.find(name);
    if (it != soundEffects.end())
    {
        auto &audio = it->second;
        audio->stop();
        audio->play();
    }
    else
    {
        std::cerr << "Sound not found: " << name << std::endl;
    }
}

void SoundManager::stopSound(const std::string &name)
{
    auto it = soundEffects.find(name);
    if (it != soundEffects.end())
    {
        it->second->stop();
    }
    else
    {
        std::cerr << "Sound not found: " << name << std::endl;
    }
}

void SoundManager::startLoop(const std::string &name)
{
    auto it = soundEffects.find(name);
    if (it != soundEffects.end())
    {
        auto &audio = it->second;
        audio->setLoop(true);
        audio->play();
    }
    else
    {
        std::cerr << "Sound not found: " << name << std::endl;
    }
}

void SoundManager::stopLoop(const std::string &name)
{
    auto it = soundEffects.find(name);
    if (it != soundEffects.end())
    {
        auto &audio = it->second;
        audio->setLoop(false);
        audio->stop();
    }
    else
    {
        std::cerr << "Sound not found: " << name << std::endl;
    }
}

SoundManager &SoundManager::getInstance()
{
    static SoundManager instance;
    return instance;
}

void SoundManager::stopAll()
{
    for (auto &pair : soundEffects)
    {
        pair.second->stop();
    }
}

void SoundManager::pauseAll()
{
    for (auto &pair : soundEffects)
    {
        pair.second->pause();
    }
}

sf::SoundSource::Status SoundManager::getSoundStatus(const std::string &name)
{
    auto it = soundEffects.find(name);
    if (it != soundEffects.end())
    {
        return it->second->getStatus();
    }
    return sf::SoundSource::Status::Stopped;
}

bool SoundManager::isSoundPlaying(const std::string &name) {
    auto it = soundEffects.find(name);
    if (it != soundEffects.end())
    {
        return it->second->getStatus() == sf::SoundSource::Status::Playing;
    }
    return false;
}

/* AUDIO */

void Audio::play() {
    sound.play();
}

void Audio::stop() {
    sound.stop();
}

void Audio::pause() {
    sound.pause();
}

void Audio::setLoop(bool loop) {
    sound.setLooping(loop);
}

sf::SoundSource::Status Audio::getStatus() {
    return sound.getStatus();
}
