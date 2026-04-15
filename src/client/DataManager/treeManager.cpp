#include "treeManager.hpp"
#include <SDL2/SDL_image.h>
#include <cmath>
#include <spdlog/spdlog.h>

TreeManager::~TreeManager()
{
    for (auto& [path, cache] : m_textureCache)
    {
        (void)path;
        if (cache.texture != nullptr) {
            SDL_DestroyTexture(cache.texture);
        }
    }
}

TreeManager& TreeManager::getInstance()
{
    static TreeManager instance;
    return instance;
}

void TreeManager::syncTrees(const std::vector<TreeInfo>& trees)
{
    m_treeMap.clear();
    for (const TreeInfo& tree : trees)
    {
        m_treeMap[tree.treeId] = tree;
    }
}

void TreeManager::updateTree(const TreeInfo& treeInfo)
{
    m_treeMap[treeInfo.treeId] = treeInfo;
}

void TreeManager::clearTrees()
{
    m_treeMap.clear();
}

void TreeManager::renderTrees(SDL_Renderer* renderer, const Camera& camera)
{
    if (renderer == nullptr) {
        return;
    }

    for (const auto& [treeId, tree] : m_treeMap)
    {
        (void)treeId;
        SDL_Texture* texture = loadTexture(renderer, tree.spritePath);
        if (texture == nullptr) {
            continue;
        }

        const TextureCache& cache = m_textureCache[tree.spritePath];
        const SDL_Point screenPoint = camera.worldToScreen(tree.x, tree.y);

        SDL_Rect srcRect{0, 0, cache.width, cache.height};
        SDL_Rect dstRect{
            screenPoint.x - static_cast<int>(cache.width * 0.75f / 2.0f),
            screenPoint.y - static_cast<int>(cache.height * 0.75f),
            static_cast<int>(cache.width * 0.75f),
            static_cast<int>(cache.height * 0.75f)
        };

        if (tree.state == ENUM_TREE_STATE_STUMP)
        {
            const int stumpHeight = std::max(32, cache.height / 3);
            srcRect.y = cache.height - stumpHeight;
            srcRect.h = stumpHeight;
            dstRect.y = screenPoint.y - static_cast<int>(stumpHeight * 0.75f);
            dstRect.h = static_cast<int>(stumpHeight * 0.75f);
        }

        SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);

        if (tree.state == ENUM_TREE_STATE_ALIVE && tree.maxHp > 0)
        {
            SDL_Rect hpBack{screenPoint.x - 30, dstRect.y - 12, 60, 6};
            SDL_Rect hpFront = hpBack;
            hpFront.w = static_cast<int>(hpBack.w * (static_cast<float>(tree.hp) / tree.maxHp));
            SDL_SetRenderDrawColor(renderer, 40, 20, 20, 220);
            SDL_RenderFillRect(renderer, &hpBack);
            SDL_SetRenderDrawColor(renderer, 88, 184, 96, 240);
            SDL_RenderFillRect(renderer, &hpFront);
        }
    }
}

int TreeManager::findNearestAliveTree(int playerX, int playerY, int maxDistance) const
{
    int nearestTreeId = -1;
    float nearestDistance = static_cast<float>(maxDistance);

    for (const auto& [treeId, tree] : m_treeMap)
    {
        if (tree.state != ENUM_TREE_STATE_ALIVE) {
            continue;
        }

        const float dx = static_cast<float>(tree.x - playerX);
        const float dy = static_cast<float>(tree.y - playerY);
        const float distance = std::sqrt(dx * dx + dy * dy);
        if (distance > nearestDistance) {
            continue;
        }

        nearestDistance = distance;
        nearestTreeId = treeId;
    }

    return nearestTreeId;
}

SDL_Texture* TreeManager::loadTexture(SDL_Renderer* renderer, const std::string& spritePath)
{
    auto iter = m_textureCache.find(spritePath);
    if (iter != m_textureCache.end()) {
        return iter->second.texture;
    }

    TextureCache cache;
    cache.texture = IMG_LoadTexture(renderer, spritePath.c_str());
    if (cache.texture == nullptr)
    {
        spdlog::error("加载树木图片失败: {} {}", spritePath, IMG_GetError());
        m_textureCache[spritePath] = cache;
        return nullptr;
    }

    SDL_QueryTexture(cache.texture, nullptr, nullptr, &cache.width, &cache.height);
    m_textureCache[spritePath] = cache;
    return cache.texture;
}
