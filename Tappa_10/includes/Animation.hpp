// https://stackoverflow.com/questions/52655335/animating-sprites-in-sfml-from-a-sprite-sheet
#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "global_values.hpp"

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

    Animation(sf::Sprite &target);

public:
    void addFrame(Frame &&frame);
    void update(float elapsed);
    static void insertAnimation(Direction dir, std::map<Direction, Animation> &animMap, const std::map<Direction, sf::Vector2i> &texMap, sf::Sprite &target, float offset, float frameDuration = 0.1f);
};