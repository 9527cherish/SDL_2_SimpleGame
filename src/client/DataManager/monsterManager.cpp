#include "monsterManager.hpp"

#include <SDL2/SDL_image.h>
#include <cmath>
#include <set>
#include <spdlog/spdlog.h>
#include "loadXml.hpp"

namespace {
constexpr float kMonsterScale = 1.5f;

bool hasSuffix(const std::string& value, const std::string& suffix)
{
    return value.size() >= suffix.size() &&
           value.compare(value.size() - suffix.size(), suffix.size(), suffix) == 0;
}
}

MonsterManager::~MonsterManager()
{
    for (auto& [path, cache] : m_textureCache)
    {
        (void)path;
        if (cache.texture != nullptr) {
            SDL_DestroyTexture(cache.texture);
        }
    }
}

MonsterManager& MonsterManager::getInstance()
{
    static MonsterManager instance;
    return instance;
}

void MonsterManager::syncMonsters(const std::vector<MonsterInfo>& monsters)
{
    std::map<int, MonsterInfo> nextMonsterMap;
    std::set<int> aliveIds;
    for (const MonsterInfo& monster : monsters)
    {
        nextMonsterMap[monster.monsterId] = monster;
        aliveIds.insert(monster.monsterId);
        ensureRenderState(monster);
    }

    m_monsterMap = std::move(nextMonsterMap);

    for (auto iter = m_renderStates.begin(); iter != m_renderStates.end(); )
    {
        if (!aliveIds.count(iter->first)) {
            iter = m_renderStates.erase(iter);
            continue;
        }
        ++iter;
    }
}

void MonsterManager::updateMonster(const MonsterInfo& monster)
{
    m_monsterMap[monster.monsterId] = monster;
    ensureRenderState(monster);
}

void MonsterManager::clearMonsters()
{
    m_monsterMap.clear();
    m_renderStates.clear();
}

void MonsterManager::renderMonsters(SDL_Renderer* renderer, const Camera& camera, Uint32 worldTime)
{
    if (renderer == nullptr) {
        return;
    }

    if (m_lastRenderTime == 0) {
        m_lastRenderTime = worldTime;
    }
    const Uint32 deltaTime = worldTime - m_lastRenderTime;
    m_lastRenderTime = worldTime;

    for (const auto& [monsterId, monster] : m_monsterMap)
    {
        MonsterRenderState& renderState = m_renderStates[monsterId];
        ensureRenderState(monster);

        const int dx = monster.x - renderState.lastX;
        const int dy = monster.y - renderState.lastY;
        if (std::abs(dx) > std::abs(dy) && dx != 0) {
            renderState.direction = dx > 0 ? CharaDirection::RIGHT : CharaDirection::LEFT;
        } else if (dy != 0) {
            renderState.direction = dy > 0 ? CharaDirection::DOWN : CharaDirection::UP;
        }

        const bool moving = dx != 0 || dy != 0;
        const CharaAction nextAction =
            monster.state == ENUM_MONSTER_STATE_DEAD
                ? CharaAction::DEAD
                : (moving ? CharaAction::WALK : CharaAction::STAND);

        if (renderState.action != nextAction)
        {
            renderState.action = nextAction;
            for (PartBase& part : renderState.parts)
            {
                part.reset(renderState.action, renderState.direction);
            }
        }

        if (!renderState.parts.empty())
        {
            for (PartBase& part : renderState.parts)
            {
                part.initTexture(renderer);
                part.update(renderState.action, renderState.direction, static_cast<int>(deltaTime));
            }

            const SDL_Point screenPoint = camera.worldToScreen(monster.x, monster.y);
            const float bobOffset = monster.state == ENUM_MONSTER_STATE_ALIVE
                ? std::sin(static_cast<float>(worldTime + monster.monsterId * 77) / 220.0f) * 1.5f
                : 0.0f;
            int drawX = static_cast<int>(screenPoint.x - 32.0f * kMonsterScale);
            int drawY = static_cast<int>(screenPoint.y - 64.0f * kMonsterScale + bobOffset);
            CharaDirection renderDirection = renderState.direction;

            for (PartBase& part : renderState.parts)
            {
                part.renderScaled(renderer, renderState.action, renderDirection, drawX, drawY, kMonsterScale);
            }

            if (monster.state == ENUM_MONSTER_STATE_ALIVE && monster.maxHp > 0)
            {
                const SDL_Rect bounds = renderBounds(renderState.parts, renderState.action, renderState.direction,
                                                     drawX, drawY, kMonsterScale);
                SDL_Rect hpBack{screenPoint.x - 30, bounds.y - 12, 60, 6};
                SDL_Rect hpFront = hpBack;
                hpFront.w = static_cast<int>(hpBack.w * (static_cast<float>(monster.hp) / monster.maxHp));
                SDL_SetRenderDrawColor(renderer, 28, 12, 12, 220);
                SDL_RenderFillRect(renderer, &hpBack);
                SDL_SetRenderDrawColor(renderer, 192, 68, 68, 240);
                SDL_RenderFillRect(renderer, &hpFront);
            }

            renderState.lastX = monster.x;
            renderState.lastY = monster.y;
            renderState.initialized = true;
            continue;
        }

        if (monster.state != ENUM_MONSTER_STATE_ALIVE) {
            renderState.lastX = monster.x;
            renderState.lastY = monster.y;
            renderState.initialized = true;
            continue;
        }

        SDL_Texture* texture = loadTexture(renderer, monster.spritePath);
        if (texture != nullptr)
        {
            const TextureCache& cache = m_textureCache[monster.spritePath];
            const SDL_Point screenPoint = camera.worldToScreen(monster.x, monster.y);
            const float bobOffset = std::sin(static_cast<float>(worldTime + monster.monsterId * 77) / 220.0f) * 3.0f;

            SDL_Rect srcRect{0, 0, cache.width, cache.height};
            SDL_Rect dstRect{
                screenPoint.x - static_cast<int>(cache.width * 0.75f / 2.0f),
                screenPoint.y - static_cast<int>(cache.height * 0.75f) + static_cast<int>(bobOffset),
                static_cast<int>(cache.width * 0.75f),
                static_cast<int>(cache.height * 0.75f)
            };

            SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);

            if (monster.maxHp > 0)
            {
                SDL_Rect hpBack{screenPoint.x - 30, dstRect.y - 12, 60, 6};
                SDL_Rect hpFront = hpBack;
                hpFront.w = static_cast<int>(hpBack.w * (static_cast<float>(monster.hp) / monster.maxHp));
                SDL_SetRenderDrawColor(renderer, 28, 12, 12, 220);
                SDL_RenderFillRect(renderer, &hpBack);
                SDL_SetRenderDrawColor(renderer, 192, 68, 68, 240);
                SDL_RenderFillRect(renderer, &hpFront);
            }
        }

        renderState.lastX = monster.x;
        renderState.lastY = monster.y;
        renderState.initialized = true;
    }
}

int MonsterManager::findNearestAliveMonster(int playerX, int playerY, int maxDistance) const
{
    int nearestMonsterId = -1;
    float nearestDistance = static_cast<float>(maxDistance);

    for (const auto& [monsterId, monster] : m_monsterMap)
    {
        if (monster.state != ENUM_MONSTER_STATE_ALIVE) {
            continue;
        }

        const float dx = static_cast<float>(monster.x - playerX);
        const float dy = static_cast<float>(monster.y - playerY);
        const float distance = std::sqrt(dx * dx + dy * dy);
        if (distance > nearestDistance) {
            continue;
        }

        nearestDistance = distance;
        nearestMonsterId = monsterId;
    }

    return nearestMonsterId;
}

bool MonsterManager::getMonster(int monsterId, MonsterInfo& monster) const
{
    auto iter = m_monsterMap.find(monsterId);
    if (iter == m_monsterMap.end()) {
        return false;
    }
    monster = iter->second;
    return true;
}

std::vector<SDL_Rect> MonsterManager::collisionRects() const
{
    std::vector<SDL_Rect> rects;
    rects.reserve(m_monsterMap.size());
    for (const auto& [monsterId, monster] : m_monsterMap)
    {
        (void)monsterId;
        if (monster.state != ENUM_MONSTER_STATE_ALIVE) {
            continue;
        }
        rects.push_back(SDL_Rect{monster.x - 18, monster.y - 14, 36, 14});
    }
    return rects;
}

SDL_Texture* MonsterManager::loadTexture(SDL_Renderer* renderer, const std::string& spritePath)
{
    auto iter = m_textureCache.find(spritePath);
    if (iter != m_textureCache.end()) {
        return iter->second.texture;
    }

    TextureCache cache;
    cache.texture = IMG_LoadTexture(renderer, spritePath.c_str());
    if (cache.texture == nullptr)
    {
        spdlog::error("加载怪物图片失败: {} {}", spritePath, IMG_GetError());
        m_textureCache[spritePath] = cache;
        return nullptr;
    }

    SDL_QueryTexture(cache.texture, nullptr, nullptr, &cache.width, &cache.height);
    m_textureCache[spritePath] = cache;
    return cache.texture;
}

void MonsterManager::ensureRenderState(const MonsterInfo& monster)
{
    MonsterRenderState& renderState = m_renderStates[monster.monsterId];
    if (!renderState.initialized)
    {
        renderState.lastX = monster.x;
        renderState.lastY = monster.y;
        renderState.direction = CharaDirection::DOWN;
        renderState.action = monster.state == ENUM_MONSTER_STATE_DEAD ? CharaAction::DEAD : CharaAction::STAND;
    }

    if (renderState.spritePath == monster.spritePath && !renderState.parts.empty()) {
        return;
    }

    renderState.parts.clear();
    renderState.spritePath = monster.spritePath;
    if (!hasSuffix(monster.spritePath, ".xml")) {
        return;
    }

    auto iter = m_partTemplates.find(monster.spritePath);
    if (iter == m_partTemplates.end())
    {
        iter = m_partTemplates.emplace(monster.spritePath, buildPartTemplate(monster.spritePath)).first;
    }

    renderState.parts = iter->second;
    for (PartBase& part : renderState.parts)
    {
        part.reset(renderState.action, renderState.direction);
    }
}

std::vector<PartBase> MonsterManager::buildPartTemplate(const std::string& spritePath) const
{
    std::vector<PartBase> parts;

    for (const std::string& accessoryPath : accessorySpritePaths(spritePath))
    {
        PartBase accessoryPart;
        LoadXml::parsePartBaseXml(accessoryPath, accessoryPart);
        if (accessoryPart.imageSet().frameWidth > 0 || accessoryPart.spriteData().frameWidth > 0) {
            parts.emplace_back(accessoryPart);
        }
    }

    PartBase basePart;
    LoadXml::parsePartBaseXml(spritePath, basePart);
    if (basePart.imageSet().frameWidth > 0 || basePart.spriteData().frameWidth > 0) {
        parts.emplace_back(basePart);
    }
    return parts;
}

std::vector<std::string> MonsterManager::accessorySpritePaths(const std::string& spritePath) const
{
    if (hasSuffix(spritePath, "/slime.xml")) {
        return {"../images/tmwa/graphics/sprites/monsters/accessories/generic-shadow-small-circle.xml"};
    }

    if (hasSuffix(spritePath, "/bee.xml")) {
        return {"../images/tmwa/graphics/sprites/monsters/accessories/generic-shadow-small-circle.xml"};
    }

    return {};
}

SDL_Rect MonsterManager::renderBounds(const std::vector<PartBase>& parts,
                                      const CharaAction& actionName,
                                      const CharaDirection& direction,
                                      int drawX,
                                      int drawY,
                                      float scale) const
{
    bool initialized = false;
    SDL_Rect bounds{drawX, drawY, static_cast<int>(64 * scale), static_cast<int>(64 * scale)};

    for (const PartBase& part : parts)
    {
        SDL_Rect rect = part.renderRect(actionName, direction, drawX, drawY);
        rect.x = static_cast<int>(drawX + (rect.x - drawX) * scale);
        rect.y = static_cast<int>(drawY + (rect.y - drawY) * scale);
        rect.w = static_cast<int>(rect.w * scale);
        rect.h = static_cast<int>(rect.h * scale);

        if (!initialized)
        {
            bounds = rect;
            initialized = true;
            continue;
        }

        const int left = std::min(bounds.x, rect.x);
        const int top = std::min(bounds.y, rect.y);
        const int right = std::max(bounds.x + bounds.w, rect.x + rect.w);
        const int bottom = std::max(bounds.y + bounds.h, rect.y + rect.h);
        bounds.x = left;
        bounds.y = top;
        bounds.w = right - left;
        bounds.h = bottom - top;
    }

    return bounds;
}
