#pragma once

#include "createCharacter.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class PartBase
{
public:
    virtual void update(const std::string& actionName, const Direction& direction, int deltaTime) = 0;
    virtual void reset(const std::string& actionName, const Direction& direction) = 0;
    virtual void render(SDL_Renderer* renderer,const std::string& actionName, 
                            Direction& dir, int& x, int& y) = 0;
};