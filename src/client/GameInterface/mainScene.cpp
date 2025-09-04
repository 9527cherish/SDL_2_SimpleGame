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

MainScene::MainScene()
{
    if (TTF_Init() == -1) {
        spdlog::error("TTF_Init 初始化失败:" + std::string(TTF_GetError()));
    }

    titleFont = TTF_OpenFont("NotoSansCJK.ttf", 16);
    if (!titleFont) {
        spdlog::error("加载字体失败:"  + std::string(TTF_GetError()));
    }

    buttonFont = TTF_OpenFont("NotoSansCJK.ttf", 16);
    if (!buttonFont) {
        spdlog::error("加载字体失败:"  + std::string(TTF_GetError()));
    }

    labelFont = TTF_OpenFont("NotoSansCJK.ttf", 16);
    if (!labelFont) {
        spdlog::error("加载字体失败:"  + std::string(TTF_GetError()));
    }
}

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

                
    // 渲染主菜单按钮

    startButton->renderButton();
    settingsButton->renderButton();
    exitButton->renderButton();
    
    // 渲染底部信息
    SDL_Surface* infoSurface = TTF_RenderText_Blended(labelFont, "按ESC键退出 | 版本 1.0.0", SDL_Color{100, 100, 100, 122});
    if (infoSurface) {
        SDL_Texture* infoTexture = SDL_CreateTextureFromSurface(renderer, infoSurface);
        SDL_Rect infoRect = {
            (SCREEN_WIDTH - infoSurface->w) / 2,
            SCREEN_HEIGHT - 40,
            infoSurface->w,
            infoSurface->h
        };
        SDL_RenderCopy(renderer, infoTexture, nullptr, &infoRect);
        SDL_DestroyTexture(infoTexture);
        SDL_FreeSurface(infoSurface);
    }


}

void MainScene::handleEvent()
{

}

void MainScene::initButton()
{
    SDL_Renderer* renderer = InterfaceManager::getInstance().renderer();

    int buttonWidth = 300;
    int buttonHeight = 60;
    int buttonY = SCREEN_HEIGHT / 2 - 80;
    int buttonSpacing = 80;
    
    startButton = std::make_unique<Button>(
        renderer, buttonFont, "开始游戏",
        (SCREEN_WIDTH - buttonWidth) / 2, buttonY,
        buttonWidth, buttonHeight,
        SDL_Color{108, 92, 231, 200},    // 正常颜色
        SDL_Color{108, 92, 231, 255},    // 悬停颜色
        SDL_Color{85, 72, 189, 255},     // 按下颜色
        SDL_Color{255, 255, 255}         // 文本颜色
    );

    buttonY += buttonSpacing;
    settingsButton = std::make_unique<Button>(
        renderer, buttonFont, "游戏设置",
        (SCREEN_WIDTH - buttonWidth) / 2, buttonY,
        buttonWidth, buttonHeight,
        SDL_Color{45, 52, 54, 200},      // 正常颜色
        SDL_Color{45, 52, 54, 255},      // 悬停颜色
        SDL_Color{30, 35, 36, 255},      // 按下颜色
        SDL_Color{108, 92, 231}          // 文本颜色
    );

    buttonY += buttonSpacing;
    exitButton = std::make_unique<Button>(
        renderer, buttonFont, "退出游戏",
        (SCREEN_WIDTH - buttonWidth) / 2, buttonY,
        buttonWidth, buttonHeight,
        SDL_Color{45, 52, 54, 200},      // 正常颜色
        SDL_Color{45, 52, 54, 255},      // 悬停颜色
        SDL_Color{30, 35, 36, 255},      // 按下颜色
        SDL_Color{253, 121, 168}         // 文本颜色
    );

    // 初始化设置菜单元素
    backButton = std::make_unique<Button>(
        renderer, buttonFont, "返回",
        50, 50,
        120, 50,
        SDL_Color{45, 52, 54, 200},      // 正常颜色
        SDL_Color{45, 52, 54, 255},      // 悬停颜色
        SDL_Color{30, 35, 36, 255},      // 按下颜色
        SDL_Color{255, 255, 255}         // 文本颜色
    );
    backButton->setVisible(false);

    saveButton = std::make_unique<Button>(
        renderer, buttonFont, "保存设置",
        SCREEN_WIDTH - 170, 50,
        120, 50,
        SDL_Color{108, 92, 231, 200},    // 正常颜色
        SDL_Color{108, 92, 231, 255},    // 悬停颜色
        SDL_Color{85, 72, 189, 255},     // 按下颜色
        SDL_Color{255, 255, 255}         // 文本颜色
    );
    saveButton->setVisible(false);
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
