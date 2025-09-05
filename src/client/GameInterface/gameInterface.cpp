#include "gameInterface.hpp"
#include "sceneStruct.hpp"

GameInterface::GameInterface()
    : m_binit(false)
{
}

GameInterface::~GameInterface()
{
}

void GameInterface::init()
{
    if(m_binit)
        return;

    initScene();
    m_binit = true;
}

bool GameInterface::isPointInRect(const int &x, const int &y, const SDL_Rect &rect)
{
    return (x >= rect.x && x <= rect.x + rect.w &&
        y >= rect.y && y <= rect.y + rect.h);
}
