#pragma once

#include "sceneStruct.hpp"
#include <SDL2/SDL.h>

class Camera
{
public:
    void follow(int targetX, int targetY);
    int x() const;
    int y() const;
    SDL_Rect backgroundSourceRect(int backgroundWidth, int backgroundHeight) const;
    SDL_Point worldToScreen(int worldX, int worldY) const;

private:
    int m_x = SCREEN_WIDTH / 2;
    int m_y = SCREEN_HEIGHT / 2;
};
