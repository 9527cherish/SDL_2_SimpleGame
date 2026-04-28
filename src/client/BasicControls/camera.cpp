#include "camera.hpp"
#include <algorithm>

void Camera::follow(int targetX, int targetY)
{
    m_x = targetX;
    m_y = targetY;
}

int Camera::x() const
{
    return m_x;
}

int Camera::y() const
{
    return m_y;
}

SDL_Rect Camera::backgroundSourceRect(int backgroundWidth, int backgroundHeight) const
{
    const int maxX = std::max(0, backgroundWidth - SCREEN_WIDTH);
    const int maxY = std::max(0, backgroundHeight - SCREEN_HEIGHT);

    return SDL_Rect{
        std::clamp(m_x - SCREEN_WIDTH / 2, 0, maxX),
        std::clamp(m_y - SCREEN_HEIGHT / 2, 0, maxY),
        std::min(SCREEN_WIDTH, backgroundWidth),
        std::min(SCREEN_HEIGHT, backgroundHeight)
    };
}

SDL_Point Camera::worldToScreen(int worldX, int worldY) const
{
    return SDL_Point{
        SCREEN_WIDTH / 2 + (worldX - m_x),
        SCREEN_HEIGHT / 2 + (worldY - m_y)
    };
}
