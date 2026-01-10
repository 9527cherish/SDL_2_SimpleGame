
#include "cursor.hpp"
#include "SDL_events.h"

Cursor::Cursor()
{
}

Cursor::~Cursor()
{
}

void Cursor::reloadCursor()
{
}

void Cursor::initCursor()
{
}

bool Cursor::handleEvent(const SDL_Event &e)
{
    if (e.type == SDL::SDL_MOUSEBUTTONDOWN && cursorClicked) 
        SDL_SetCursor(cursorClicked);
    else
    {
        if (e.type == SDL::SDL_MOUSEBUTTONUP && cursor) {
            SDL_SetCursor(cursor);
        }
    }

}
