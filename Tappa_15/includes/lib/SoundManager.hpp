#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <memory>

struct Audio {
    private:
        sf::SoundBuffer buffer;
        sf::Sound sound;
    
    public:
        Audio(sf::SoundBuffer buf) : buffer(buf), sound(buffer) {}
        void play();
        void stop();
        void pause();
        void setLoop(bool loop);
        sf::SoundSource::Status getStatus();
};

struct SoundManager
{
    std::map<std::string, Audio*> soundEffects;
    SoundManager();

    int wakaSound = 0;

private:
    void loadAll();
    void loadSound(const std::string &name, const std::string &filePath);

public:
    void playSound(const std::string &name);
    void stopSound(const std::string &name);
    void startLoop(const std::string &name);
    void stopLoop(const std::string &name);
    void stopAll();
    void pauseAll();
    static SoundManager &getInstance();
    sf::SoundSource::Status getSoundStatus(const std::string &name);
    bool isSoundPlaying(const std::string &name);
};