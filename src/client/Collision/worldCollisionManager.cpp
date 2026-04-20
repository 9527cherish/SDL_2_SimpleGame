#include "Collision/worldCollisionManager.hpp"

#include <SDL2/SDL.h>
#include <algorithm>
#include <vector>
#include "monsterManager.hpp"
#include "treeManager.hpp"

namespace {
SDL_Rect playerFootbox(int centerX, int footY)
{
    return SDL_Rect{centerX - 12, footY - 10, 24, 10};
}

bool intersectsAny(const SDL_Rect& target, const std::vector<SDL_Rect>& obstacles)
{
    for (const SDL_Rect& obstacle : obstacles)
    {
        if (SDL_HasIntersection(&target, &obstacle)) {
            return true;
        }
    }
    return false;
}
}

WorldCollisionManager& WorldCollisionManager::getInstance()
{
    static WorldCollisionManager instance;
    return instance;
}

void WorldCollisionManager::setWorldBounds(int width, int height)
{
    m_worldWidth = std::max(width, 1);
    m_worldHeight = std::max(height, 1);
}

bool WorldCollisionManager::canOccupyPlayerFootbox(int centerX, int footY) const
{
    SDL_Rect footbox = playerFootbox(centerX, footY);
    if (footbox.x < 0 || footbox.y < 0 ||
        footbox.x + footbox.w > m_worldWidth ||
        footbox.y + footbox.h > m_worldHeight) {
        return false;
    }

    if (intersectsAny(footbox, TreeManager::getInstance().collisionRects())) {
        return false;
    }

    if (intersectsAny(footbox, MonsterManager::getInstance().collisionRects())) {
        return false;
    }

    return true;
}
