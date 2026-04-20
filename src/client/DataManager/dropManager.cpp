#include "dropManager.hpp"

#include <SDL2/SDL_image.h>
#include <cmath>
#include <spdlog/spdlog.h>

DropManager::~DropManager()
{
    for (auto& [path, cache] : m_textureCache)
    {
        (void)path;
        if (cache.texture != nullptr) {
            SDL_DestroyTexture(cache.texture);
        }
    }
}

DropManager& DropManager::getInstance()
{
    static DropManager instance;
    return instance;
}

void DropManager::syncDrops(const std::vector<DropInfo>& drops)
{
    m_dropMap.clear();
    for (const DropInfo& drop : drops)
    {
        m_dropMap[drop.dropId] = drop;
    }
}

void DropManager::updateDrop(const DropInfo& drop)
{
    m_dropMap[drop.dropId] = drop;
}

void DropManager::clearDrops()
{
    m_dropMap.clear();
}

void DropManager::renderDrops(SDL_Renderer* renderer, const Camera& camera, Uint32 worldTime)
{
    if (renderer == nullptr) {
        return;
    }

    for (const auto& [dropId, drop] : m_dropMap)
    {
        (void)dropId;
        if (drop.state != ENUM_DROP_STATE_AVAILABLE) {
            continue;
        }

        SDL_Texture* texture = loadTexture(renderer, drop.spritePath);
        if (texture == nullptr) {
            continue;
        }

        const TextureCache& cache = m_textureCache[drop.spritePath];
        const SDL_Point screenPoint = camera.worldToScreen(drop.x, drop.y);
        const float bobOffset = std::sin(static_cast<float>(worldTime + drop.dropId * 53) / 180.0f) * 2.0f;

        SDL_Rect srcRect{0, 0, cache.width, cache.height};
        SDL_Rect dstRect{
            screenPoint.x - static_cast<int>(cache.width * 0.9f / 2.0f),
            screenPoint.y - static_cast<int>(cache.height * 0.9f) + static_cast<int>(bobOffset),
            static_cast<int>(cache.width * 0.9f),
            static_cast<int>(cache.height * 0.9f)
        };

        SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
    }
}

int DropManager::findNearestAvailableDrop(int playerX, int playerY, int maxDistance) const
{
    int nearestDropId = -1;
    float nearestDistance = static_cast<float>(maxDistance);

    for (const auto& [dropId, drop] : m_dropMap)
    {
        if (drop.state != ENUM_DROP_STATE_AVAILABLE) {
            continue;
        }

        const float dx = static_cast<float>(drop.x - playerX);
        const float dy = static_cast<float>(drop.y - playerY);
        const float distance = std::sqrt(dx * dx + dy * dy);
        if (distance > nearestDistance) {
            continue;
        }

        nearestDistance = distance;
        nearestDropId = dropId;
    }

    return nearestDropId;
}

SDL_Texture* DropManager::loadTexture(SDL_Renderer* renderer, const std::string& spritePath)
{
    auto iter = m_textureCache.find(spritePath);
    if (iter != m_textureCache.end()) {
        return iter->second.texture;
    }

    TextureCache cache;
    cache.texture = IMG_LoadTexture(renderer, spritePath.c_str());
    if (cache.texture == nullptr)
    {
        spdlog::error("加载掉落物图片失败: {} {}", spritePath, IMG_GetError());
        m_textureCache[spritePath] = cache;
        return nullptr;
    }

    SDL_QueryTexture(cache.texture, nullptr, nullptr, &cache.width, &cache.height);
    m_textureCache[spritePath] = cache;
    return cache.texture;
}
