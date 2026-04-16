#pragma once

#include "sceneStruct.hpp"
#include <SDL2/SDL.h>

// Camera 负责把世界坐标转换为屏幕坐标，并提供背景裁切区域
class Camera
{
public:
    // 让相机跟随目标世界坐标
    void follow(int targetX, int targetY);
    // 返回相机当前世界坐标 X
    int x() const;
    // 返回相机当前世界坐标 Y
    int y() const;
    // 计算背景大图在当前相机下应裁切的源区域
    SDL_Rect backgroundSourceRect(int backgroundWidth, int backgroundHeight) const;
    // 把世界坐标换算成当前屏幕坐标
    SDL_Point worldToScreen(int worldX, int worldY) const;

private:
    // 相机当前跟随的世界坐标 X
    int m_x = SCREEN_WIDTH / 2;
    // 相机当前跟随的世界坐标 Y
    int m_y = SCREEN_HEIGHT / 2;
};
