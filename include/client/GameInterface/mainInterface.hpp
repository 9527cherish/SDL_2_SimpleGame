#pragma once 
#include "gameInterface.hpp"

class MainInterface : public GameInterface
{
public:
    void initScene() override;


    // 绘制左右箭头
    void drawLeftArrow(SDL_Renderer* renderer, const int& x, const int& y, const int& width, const int& height);
    void drawRightArrow(SDL_Renderer* renderer, const int& x, const int& y, const int& width, const int& height);
private:

};

