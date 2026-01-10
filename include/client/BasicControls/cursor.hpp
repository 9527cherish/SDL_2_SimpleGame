#pragma once 

#include "SDL_stdinc.h"
#include "SDL_mouse.h"

#include <atomic>
#include "memory"

class Cursor
{
public:
    Cursor();
    ~Cursor();

    void reloadCursor();
    void initCursor();
    bool handleEvent(const SDL_Event& e); 

private:
    std::unique_ptr<SDL::SDL_Cursor> m_pOriginalCursor;
    std::unique_ptr<SDL::SDL_Cursor> m_pCursor;
    std::unique_ptr<SDL::SDL_Cursor> m_pCursorClicked;
    std::atomic_bool m_bClicked;

    
};
