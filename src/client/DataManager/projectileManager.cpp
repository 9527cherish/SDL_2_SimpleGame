#include "projectileManager.hpp"

ProjectileManager& ProjectileManager::getInstance()
{
    static ProjectileManager instance;
    return instance;
}

void ProjectileManager::syncProjectiles(const std::vector<ProjectileInfo>& projectiles)
{
    m_projectiles.clear();
    for (const ProjectileInfo& projectile : projectiles)
    {
        if (projectile.state != ENUM_PROJECTILE_STATE_ACTIVE) {
            continue;
        }
        m_projectiles[projectile.projectileId] = projectile;
    }
}

void ProjectileManager::updateProjectile(const ProjectileInfo& projectile)
{
    if (projectile.state == ENUM_PROJECTILE_STATE_ACTIVE)
    {
        m_projectiles[projectile.projectileId] = projectile;
        return;
    }

    m_projectiles.erase(projectile.projectileId);
}

void ProjectileManager::clear()
{
    m_projectiles.clear();
}

void ProjectileManager::renderProjectiles(SDL_Renderer* renderer, const Camera& camera)
{
    if (renderer == nullptr) {
        return;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    for (const auto& [projectileId, projectile] : m_projectiles)
    {
        (void)projectileId;
        const SDL_Point screenPoint = camera.worldToScreen(static_cast<int>(projectile.x), static_cast<int>(projectile.y));

        if (projectile.projectileType == "arrow")
        {
            SDL_SetRenderDrawColor(renderer, 220, 186, 92, 255);
            SDL_Rect arrowRect{screenPoint.x - 9, screenPoint.y - 2, 18, 4};
            SDL_RenderFillRect(renderer, &arrowRect);
        }
        else if (projectile.projectileType == "magic")
        {
            SDL_SetRenderDrawColor(renderer, 110, 206, 255, 255);
            SDL_Rect coreRect{screenPoint.x - 5, screenPoint.y - 5, 10, 10};
            SDL_RenderFillRect(renderer, &coreRect);
            SDL_SetRenderDrawColor(renderer, 180, 110, 255, 180);
            SDL_Rect auraRect{screenPoint.x - 9, screenPoint.y - 9, 18, 18};
            SDL_RenderDrawRect(renderer, &auraRect);
        }
    }
}
