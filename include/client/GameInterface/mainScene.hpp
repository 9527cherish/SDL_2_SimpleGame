#pragma once 


#include "gameInterface.hpp"
#include "button.hpp"
#include <memory>

class MainScene : public GameInterface
{
public:
    MainScene();
    
    void handleEvent(const SDL_Event &e) override;

    // 初始化所有控件
    void initButton();
    void initScene() override;
    
    // 渲染控件
    void renderScene() override;
    void renderButton();
    void renderArrow();

    // 绘制左右箭头
    void drawLeftArrow(SDL_Renderer* renderer, const int& x, const int& y, const int& width, const int& height);
    void drawRightArrow(SDL_Renderer* renderer, const int& x, const int& y, const int& width, const int& height);
private:
    // 主菜单按钮
    std::unique_ptr<Button> m_pStartButton;
    std::unique_ptr<Button> m_pSettingsButton;
    std::unique_ptr<Button> m_pExitButton;

        // 设置菜单元素
    std::unique_ptr<Button> m_pBackButton;
    std::unique_ptr<Button> m_pSaveButton;
    
    TTF_Font* m_pTitleFont;
    TTF_Font* m_pButtonFont;
    TTF_Font* m_pLabelFont;

    SDL_Renderer* m_pRenderer;

};

