#include "../../includes/core/Animation.hpp"
#include <iostream>

Animation::Animation(sf::Sprite &target) : target(&target), totalProgess(0), currentFrame(0) {}

void Animation::addFrame(Frame &&frame)
{
    frames.push_back(std::move(frame));
    totalLength += frame.duration;
}

void Animation::update(float elapsed)
{
    if (frames.empty() || (finished && !loop))
        return;

    totalProgess += elapsed;

    while (totalProgess >= frames[currentFrame].duration)
    {
        totalProgess -= frames[currentFrame].duration;
        currentFrame++;
        if (currentFrame >= frames.size())
        {
            if (loop)
                currentFrame = 0;
            else
            {
                finished = true;
                return;
            }
        }
    }

    target->setTextureRect(frames[currentFrame].rect);
}

void Animation::insertAnimation(Direction dir, std::map<Direction, Animation> &animMap, const std::map<Direction, sf::Vector2i> &texMap, sf::Sprite &target, float offset, float frameDuration)
{
    animMap.insert({dir, Animation(target)});
    std::map<Direction, Animation>::iterator it = animMap.find(dir);
    if (it != animMap.end())
    {
        it->second.addFrame({sf::IntRect(
                                 {static_cast<int>(texMap.at(dir).x + offset) * (TILE_SIZE / 2), (texMap.at(dir).y) * (TILE_SIZE / 2)},
                                 {(TILE_SIZE / 2), (TILE_SIZE / 2)}),
                             frameDuration});
        it->second.addFrame({sf::IntRect(
                                 {texMap.at(dir).x * (TILE_SIZE / 2), texMap.at(dir).y * (TILE_SIZE / 2)},
                                 {(TILE_SIZE / 2), (TILE_SIZE / 2)}),
                             frameDuration});
    }
}

void Animation::insertAnimation(std::vector<Animation> &animMap, sf::Vector2i tex, sf::Sprite &target, float frameDuration)
{
    animMap.emplace_back(target);
    Animation &anim = animMap.back();
    anim.addFrame({sf::IntRect(
                       {tex.x * (TILE_SIZE / 2), tex.y * (TILE_SIZE / 2)},
                       {(TILE_SIZE / 2), (TILE_SIZE / 2)}),
                   frameDuration});
}

void Animation::insertAnimation(std::vector<Animation> &animMap, std::vector<sf::Vector2i> texVec, sf::Sprite &target, float frameDuration, bool loop)
{
    animMap.emplace_back(target);
    Animation &anim = animMap.back();
    anim.loop = loop;
    for (const auto &tex : texVec)
    {
        anim.addFrame({sf::IntRect(
                           {tex.x * (TILE_SIZE / 2), tex.y * (TILE_SIZE / 2)},
                           {(TILE_SIZE / 2), (TILE_SIZE / 2)}),
                       frameDuration});
    }
}

bool Animation::isFinished()
{
    return currentFrame >= frames.size();
}

void Animation::reset()
{
    totalProgess = 0;
    currentFrame = 0;
    finished = false;
}