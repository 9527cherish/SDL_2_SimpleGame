#include "gameInterface.hpp"

GameInterface::gameInterface()
{
}

GameInterface::~gameInterface()
{
}

bool GameInterface::isPointInRect(const int &x, const int &y, const SDL_Rect &rect)
{
    return (x >= rect.x && x <= rect.x + rect.w &&
        y >= rect.y && y <= rect.y + rect.h);
}
