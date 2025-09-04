#pragma once 


#include "gameInterface.hpp"
#include "button.hpp"
#include <memory>

class MainScene : public GameInterface
{
public:
    MainScene();
    void renderScene() override;
    void handleEvent() override;

    void initButton();


    // 绘制左右箭头
    void drawLeftArrow(SDL_Renderer* renderer, const int& x, const int& y, const int& width, const int& height);
    void drawRightArrow(SDL_Renderer* renderer, const int& x, const int& y, const int& width, const int& height);
private:
    // 主菜单按钮
    std::unique_ptr<Button> startButton;
    std::unique_ptr<Button> settingsButton;
    std::unique_ptr<Button> exitButton;

        // 设置菜单元素
    std::unique_ptr<Button> backButton;
    std::unique_ptr<Button> saveButton;
    
    TTF_Font* titleFont;
    TTF_Font* buttonFont;
    TTF_Font* labelFont;

};

