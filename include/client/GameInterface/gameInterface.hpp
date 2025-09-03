#pragma once 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class GameInterface
{
public:
    GameInterface();
    ~GameInterface();

    virtual void renderScene() = 0;
    virtual void handleEvent() = 0;

    virtual bool isPointInRect(const int& x,const  int& y, const  SDL_Rect& rect);

// 使用  protected,子类可以访问
protected:

};

