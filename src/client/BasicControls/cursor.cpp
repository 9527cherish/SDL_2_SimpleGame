
#include "cursor.hpp"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_image.h"
#include "clientComonFunc.hpp"

Cursor::Cursor()
{
}

Cursor::~Cursor()
{
    freeCursor();
}

void Cursor::freeCursor()
{
    if (m_pCursor != nullptr)
    {
        SDL_FreeCursor(m_pCursor);
        m_pCursor = nullptr;
    }

    if (m_pCursorClicked != nullptr)
    {
        SDL_FreeCursor(m_pCursorClicked);
        m_pCursorClicked = nullptr;
    }

    if (m_pOriginalCursor != nullptr) {
        SDL_SetCursor(m_pOriginalCursor);
    }
}

void Cursor::reloadCursor()
{
}

void Cursor::initCursor()
{
    SDL_Surface* surface = IMG_Load("/home/wj/Code/SDL2__Game/SimpleGame/images/data_free/images/mouse_cursor.png");
    if (surface != nullptr)
    {
        SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);
        m_pCursor = SDL_CreateColorCursor(surface, 0, 0);
        SDL_FreeSurface(surface);
    }
    else
    {
        spdlog::error("Failed to load cursor image: {}", IMG_GetError());
    }

    if(nullptr != m_pCursor)
    {
        SDL_ShowCursor(SDL_ENABLE);
        SDL_SetCursor(m_pCursor);
    }
    else
    {
        spdlog::error("Failed to load cursor image" +  std::string(SDL_GetError()));
    }

    surface = IMG_Load("/home/wj/Code/SDL2__Game/SimpleGame/images/data_free/images/mouse_cursor2.png");
    if (surface != nullptr)
    {
        m_pCursorClicked = SDL_CreateColorCursor(surface, 0, 0);
        SDL_FreeSurface(surface);
    }
    else
    {
        spdlog::error("Failed to load cursor image: {}", IMG_GetError());
    }

    m_pOriginalCursor = SDL_GetCursor();
}

bool Cursor::handleEvent(const SDL_Event &e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN && m_pCursorClicked) 
        SDL_SetCursor(m_pCursorClicked);
    else if (e.type == SDL_MOUSEBUTTONUP && m_pCursor)
    {
        SDL_SetCursor(m_pCursor);
    }

    return true;
}
