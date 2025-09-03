#include "mainScene.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "sceneStruct.hpp"
#include "interfaceManager.hpp"

// 翻页按钮尺寸和位置
const int BUTTON_WIDTH = 50;
const int BUTTON_HEIGHT = 50;
const int BUTTON_Y = SCREEN_HEIGHT - 70;
const int LEFT_BUTTON_X = (SCREEN_WIDTH / 2) - 70;  // 左侧按钮X坐标
const int RIGHT_BUTTON_X = (SCREEN_WIDTH / 2) + 20; // 右侧按钮X坐标




void MainScene::renderScene()
{

    SDL_Renderer* renderer = InterfaceManager::getInstance().renderer();
    if(nullptr == renderer)
        return;
    // 绘制翻页按钮背景
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255); // 浅灰色
    SDL_Rect leftButtonBg = {LEFT_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_Rect rightButtonBg = {RIGHT_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_RenderFillRect(renderer, &leftButtonBg);
    SDL_RenderFillRect(renderer, &rightButtonBg);
    
    // 绘制按钮边框
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // 灰色
    SDL_RenderDrawRect(renderer, &leftButtonBg);
    SDL_RenderDrawRect(renderer, &rightButtonBg);
    
    // 绘制翻页箭头图标
    drawLeftArrow(renderer, LEFT_BUTTON_X + 10, BUTTON_Y + 10, 30, 30);
    drawRightArrow(renderer, RIGHT_BUTTON_X + 10, BUTTON_Y + 10, 30, 30);
    
    // 绘制提示文字（用矩形模拟）
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255); // 浅灰色
    SDL_Rect hintRect = {20, 20, 250, 30};
    SDL_RenderFillRect(renderer, &hintRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 黑色
    SDL_RenderDrawRect(renderer, &hintRect);

    // 更新屏幕
    SDL_RenderPresent(renderer);

}

void MainScene::handleEvent()
{

}

void MainScene::drawLeftArrow(SDL_Renderer *renderer, const int& x, const int& y, const int& width, const int& height)
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

void MainScene::drawRightArrow(SDL_Renderer *renderer, const int& x, const int& y, const int& width, const int& height)
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
