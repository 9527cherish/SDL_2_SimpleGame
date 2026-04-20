#pragma once

#include <SDL2/SDL.h>
#include <map>
#include <vector>
#include "camera.hpp"
#include "messageInfo.hpp"

class ProjectileManager
{
public:
    static ProjectileManager& getInstance();

    void syncProjectiles(const std::vector<ProjectileInfo>& projectiles);
    void updateProjectile(const ProjectileInfo& projectile);
    void clear();
    void renderProjectiles(SDL_Renderer* renderer, const Camera& camera);

private:
    ProjectileManager() = default;
    ProjectileManager(const ProjectileManager&) = delete;
    ProjectileManager& operator=(const ProjectileManager&) = delete;

private:
    std::map<int, ProjectileInfo> m_projectiles;
};
