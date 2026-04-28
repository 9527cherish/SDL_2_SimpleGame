
#include "cursor.hpp"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_image.h"
#include "clientComonFunc.hpp"

Cursor::Cursor()
    : m_pOriginalCursor(nullptr)
    , m_pCursor(nullptr)
    , m_pCursorClicked(nullptr)
{

}

Cursor::~Cursor()
{

}

void Cursor::freeCursor()
{
    delete m_pCursor;
    delete m_pCursorClicked;
}

void Cursor::reloadCursor()
{
}

void Cursor::initCursor()
{
    auto surface = IMG_Load("/home/wj/Code/SDL2__Game/SimpleGame/images/data_free/images/mouse_cursor.png");
    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);
    if (nullptr != surface) 
        m_pCursor = SDL_CreateColorCursor(surface, 0, 0);
    else
        spdlog::error("Failed to load cursor image: %s", IMG_GetError());

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
    if (nullptr != surface)
        m_pCursorClicked = SDL_CreateColorCursor(surface, 0, 0);
    else
        spdlog::error("Failed to load cursor image: %s", IMG_GetError());

    m_pOriginalCursor = SDL_GetCursor();
    // SDL_FreeSurface(surface);
}

bool Cursor::handleEvent(const SDL_Event &e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN && m_pCursorClicked) 
        SDL_SetCursor(m_pCursorClicked);
    else
    {
        if (e.type == SDL_MOUSEBUTTONUP && m_pCursor) {
            SDL_SetCursor(m_pCursor);
        }
    }
}
