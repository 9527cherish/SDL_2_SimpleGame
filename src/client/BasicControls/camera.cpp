#include "camera.hpp"
#include <algorithm>

void Camera::follow(int targetX, int targetY)
{
    m_x = targetX;
    m_y = targetY;
}

void Camera::setWorldBounds(int worldWidth, int worldHeight)
{
    m_worldWidth = std::max(worldWidth, SCREEN_WIDTH);
    m_worldHeight = std::max(worldHeight, SCREEN_HEIGHT);
}

int Camera::x() const
{
    return clampedCenter().x;
}

int Camera::y() const
{
    return clampedCenter().y;
}

SDL_Rect Camera::backgroundSourceRect(int backgroundWidth, int backgroundHeight) const
{
    const SDL_Point center = clampedCenter();
    const int maxX = std::max(0, backgroundWidth - SCREEN_WIDTH);
    const int maxY = std::max(0, backgroundHeight - SCREEN_HEIGHT);

    return SDL_Rect{
        std::clamp(center.x - SCREEN_WIDTH / 2, 0, maxX),
        std::clamp(center.y - SCREEN_HEIGHT / 2, 0, maxY),
        std::min(SCREEN_WIDTH, backgroundWidth),
        std::min(SCREEN_HEIGHT, backgroundHeight)
    };
}

SDL_Point Camera::worldToScreen(int worldX, int worldY) const
{
    const SDL_Point center = clampedCenter();
    return SDL_Point{
        SCREEN_WIDTH / 2 + (worldX - center.x),
        SCREEN_HEIGHT / 2 + (worldY - center.y)
    };
}

SDL_Point Camera::clampedCenter() const
{
    const int minCenterX = SCREEN_WIDTH / 2;
    const int minCenterY = SCREEN_HEIGHT / 2;
    const int maxCenterX = std::max(minCenterX, m_worldWidth - SCREEN_WIDTH / 2);
    const int maxCenterY = std::max(minCenterY, m_worldHeight - SCREEN_HEIGHT / 2);

    return SDL_Point{
        std::clamp(m_x, minCenterX, maxCenterX),
        std::clamp(m_y, minCenterY, maxCenterY)
    };
}
