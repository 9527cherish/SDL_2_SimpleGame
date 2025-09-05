#pragma once 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class GameInterface
{
public:
    GameInterface();

    // 析构函数声明称虚函数，否则析构的时候只会调用基类的析构函数，派生类的无法调用
    virtual ~GameInterface();

    virtual void renderScene() = 0;
    virtual void handleEvent(const SDL_Event &e) = 0;

    virtual bool isPointInRect(const int& x,const  int& y, const  SDL_Rect& rect);

// 使用  protected,子类可以访问
protected:

};

