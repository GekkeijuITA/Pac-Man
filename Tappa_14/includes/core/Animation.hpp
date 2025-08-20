// https://stackoverflow.com/questions/52655335/animating-sprites-in-sfml-from-a-sprite-sheet
#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "../lib/global_values.hpp"

struct Frame
{
    sf::IntRect rect;
    double duration;
};

struct Animation
{
    std::vector<Frame> frames;
    double totalLength, totalProgess;
    sf::Sprite *target;
    int currentFrame;
    bool loop = true, finished = false;

    Animation(sf::Sprite &target);

public:
    void addFrame(Frame &&frame);
    void update(float elapsed);
    static void insertAnimation(Direction dir, std::map<Direction, Animation> &animMap, const std::map<Direction, sf::Vector2i> &texMap, sf::Sprite &target, float offset, float frameDuration = 0.1f);
    static void insertAnimation(std::vector<Animation> &animMap, sf::Vector2i tex, sf::Sprite &target, float frameDuration = 0.1f);
    static void insertAnimation(std::vector<Animation> &animMap, std::vector<sf::Vector2i> texVec, sf::Sprite &target, float frameDuration = 0.1f, bool loop = true);
    bool isFinished();
    void reset();
};