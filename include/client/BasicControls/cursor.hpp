#pragma once 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <atomic>
#include "memory"

class Cursor
{
public:
    Cursor();
    ~Cursor();
    void freeCursor();

    void reloadCursor();
    void initCursor();
    bool handleEvent(const SDL_Event& e); 

private:
    SDL_Cursor*  m_pOriginalCursor;
    SDL_Cursor* m_pCursor;
    SDL_Cursor*  m_pCursorClicked;  
};
