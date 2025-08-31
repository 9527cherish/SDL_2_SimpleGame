#pragma once 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class GameInterface
{
public:
    gameInterface();
    ~gameInterface();

    virtual void initScene() = 0;
    virtual void handleEvent() = 0;

    bool isPointInRect(const int& x,const  int& y, const  SDL_Rect& rect);

private:
    
},

