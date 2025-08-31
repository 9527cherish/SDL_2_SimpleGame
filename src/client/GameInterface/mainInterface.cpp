#include "mainInterface.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void MainInterface::initScene()
{

}

void MainInterface::drawLeftArrow(SDL_Renderer *renderer, const int& x, const int& y, const int& width, const int& height)
{
    // 保存当前颜色
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    
    // 设置箭头颜色
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    
    // 绘制箭头三角形
    SDL_RenderDrawLine(renderer, x + width, y, x, y + height/2);
    SDL_RenderDrawLine(renderer, x, y + height/2, x + width, y + height);
    SDL_RenderDrawLine(renderer, x + width, y, x + width, y + height);
    
    // 恢复原始颜色
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void MainInterface::drawRightArrow(SDL_Renderer *renderer, const int& x, const int& y, const int& width, const int& height)
{
        // 保存当前颜色
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    
    // 设置箭头颜色
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    
    // 绘制箭头三角形
    SDL_RenderDrawLine(renderer, x, y, x + width, y + height/2);
    SDL_RenderDrawLine(renderer, x + width, y + height/2, x, y + height);
    SDL_RenderDrawLine(renderer, x, y, x, y + height);
    
    // 恢复原始颜色
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}
