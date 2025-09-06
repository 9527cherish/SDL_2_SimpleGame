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

    m_pTitleFont = TTF_OpenFont("SourceHanSansCN-Regular.otf", 16);
    if (!m_pTitleFont) {
        spdlog::error("MainScene Title加载字体失败:"  + std::string(TTF_GetError()));
    }

    m_pButtonFont = TTF_OpenFont("SourceHanSansCN-Regular.otf", 16);
    if (!m_pButtonFont) {
        spdlog::error("MainScene Button加载字体失败:"  + std::string(TTF_GetError()));
    }

    m_pLabelFont = TTF_OpenFont("SourceHanSansCN-Regular.otf", 16);
    if (!m_pLabelFont) {
        spdlog::error("MainScene Label加载字体失败:"  + std::string(TTF_GetError()));
    }
}

void MainScene::renderScene()
{
   renderArrow();
   renderButton();
}

void MainScene::handleEvent(const SDL_Event &e)
{
    m_pStartButton->handleEvent(e);  
    m_pSettingsButton->handleEvent(e);
    m_pExitButton->handleEvent(e);
}

void MainScene::initButton()
{
    if(nullptr == m_pRenderer)
        m_pRenderer = InterfaceManager::getInstance().renderer();
    
    int buttonWidth = 200;
    int buttonHeight = 60;
    int buttonX = 100;
    int buttonY = SCREEN_HEIGHT / 2 - 100;
    int buttonSpacing = 120;
    
    m_pStartButton = std::make_unique<Button>(
        m_pRenderer, m_pButtonFont, "开始游戏",
        buttonX, buttonY,
        buttonWidth, buttonHeight,
        SDL_Color{108, 92, 231, 200},    // 正常颜色
        SDL_Color{108, 92, 231, 255},    // 悬停颜色
        SDL_Color{85, 72, 189, 255},     // 按下颜色
        SDL_Color{255, 255, 255, 255}         // 文本颜色
    );

    buttonY += buttonSpacing;
    m_pSettingsButton = std::make_unique<Button>(
        m_pRenderer, m_pButtonFont, "游戏设置",
        buttonX, buttonY,
        buttonWidth, buttonHeight,
        SDL_Color{45, 52, 54, 200},      // 正常颜色
        SDL_Color{45, 52, 54, 255},      // 悬停颜色
        SDL_Color{30, 35, 36, 255},      // 按下颜色
        SDL_Color{108, 92, 231, 255}          // 文本颜色
    );

    buttonY += buttonSpacing;
    m_pExitButton = std::make_unique<Button>(
        m_pRenderer, m_pButtonFont, "退出游戏",
        buttonX, buttonY,
        buttonWidth, buttonHeight,
        SDL_Color{45, 52, 54, 200},      // 正常颜色
        SDL_Color{45, 52, 54, 255},      // 悬停颜色
        SDL_Color{30, 35, 36, 255},      // 按下颜色
        SDL_Color{253, 121, 168, 255}         // 文本颜色
    );

    // 初始化设置菜单元素
    m_pBackButton = std::make_unique<Button>(
        m_pRenderer, m_pButtonFont, "返回",
        50, 50,
        120, 50,
        SDL_Color{45, 52, 54, 200},      // 正常颜色
        SDL_Color{45, 52, 54, 255},      // 悬停颜色
        SDL_Color{30, 35, 36, 255},      // 按下颜色
        SDL_Color{255, 255, 255, 255}         // 文本颜色
    );
    m_pBackButton->setVisible(false);

    m_pSaveButton = std::make_unique<Button>(
        m_pRenderer, m_pButtonFont, "保存设置",
        SCREEN_WIDTH - 170, 50,
        120, 50,
        SDL_Color{108, 92, 231, 200},    // 正常颜色
        SDL_Color{108, 92, 231, 255},    // 悬停颜色
        SDL_Color{85, 72, 189, 255},     // 按下颜色
        SDL_Color{255, 255, 255, 255}         // 文本颜色
    );
    m_pSaveButton->setVisible(false);
}

void MainScene::renderButton()
{
    // 渲染主菜单按钮
    m_pStartButton->renderButton();
    m_pSettingsButton->renderButton();
    m_pExitButton->renderButton();
}

void MainScene::renderArrow()
{
    // 绘制翻页按钮背景
    SDL_SetRenderDrawColor(m_pRenderer, 240, 240, 240, 255); // 浅灰色
    SDL_Rect leftButtonBg = {LEFT_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_Rect rightButtonBg = {RIGHT_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_RenderFillRect(m_pRenderer, &leftButtonBg);
    SDL_RenderFillRect(m_pRenderer, &rightButtonBg);
    
    // 绘制按钮边框
    SDL_SetRenderDrawColor(m_pRenderer, 200, 200, 200, 255); // 灰色
    SDL_RenderDrawRect(m_pRenderer, &leftButtonBg);
    SDL_RenderDrawRect(m_pRenderer, &rightButtonBg);
    
    // 绘制翻页箭头图标
    drawLeftArrow(m_pRenderer, LEFT_BUTTON_X + 10, BUTTON_Y + 10, 30, 30);
    drawRightArrow(m_pRenderer, RIGHT_BUTTON_X + 10, BUTTON_Y + 10, 30, 30);
    
    // // 绘制提示文字（用矩形模拟）
    // SDL_SetRenderDrawColor(m_pRenderer, 180, 180, 180, 255); // 浅灰色
    // SDL_Rect hintRect = {20, 20, 250, 30};
    // SDL_RenderFillRect(m_pRenderer, &hintRect);
    // SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255); // 黑色
    // SDL_RenderDrawRect(m_pRenderer, &hintRect);

//     // 渲染底部信息
//     SDL_Surface* infoSurface = TTF_RenderUTF8_Blended(m_pLabelFont, "按ESC键退出 | 版本 1.0.0", SDL_Color{100, 100, 100, 122});
//     if (infoSurface) {
//         SDL_Texture* infoTexture = SDL_CreateTextureFromSurface(m_pRenderer, infoSurface);
//         SDL_Rect infoRect = {
//             (SCREEN_WIDTH - infoSurface->w) / 2,
//             SCREEN_HEIGHT - 40,
//             infoSurface->w,
//             infoSurface->h
//         };
//         SDL_RenderCopy(m_pRenderer, infoTexture, nullptr, &infoRect);
//         SDL_DestroyTexture(infoTexture);
//         SDL_FreeSurface(infoSurface);
//     }
}

void MainScene::initScene()
{
    if(nullptr == m_pRenderer)
        m_pRenderer = InterfaceManager::getInstance().renderer();

    initButton();
}

void MainScene::drawLeftArrow(SDL_Renderer *renderer, const int& x, const int& y, const int& width, const int& height)
{
    if(nullptr == m_pRenderer)
        m_pRenderer = InterfaceManager::getInstance().renderer();
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

    if(nullptr == m_pRenderer)
        m_pRenderer = InterfaceManager::getInstance().renderer();
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
