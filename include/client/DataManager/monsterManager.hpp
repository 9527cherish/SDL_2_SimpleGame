#pragma once

#include <SDL2/SDL.h>
#include <map>
#include <string>
#include <vector>
#include "camera.hpp"
#include "characterAction.hpp"
#include "characterDirection.hpp"
#include "partBase.hpp"
#include "messageInfo.hpp"

// MonsterManager 管理客户端怪物快照、纹理缓存与场景渲染
class MonsterManager
{
public:
    ~MonsterManager();
    // 返回怪物管理器单例
    static MonsterManager& getInstance();

    // 用服务器怪物快照整体替换当前客户端状态
    void syncMonsters(const std::vector<MonsterInfo>& monsters);
    // 更新单只怪物状态
    void updateMonster(const MonsterInfo& monster);
    // 清空当前怪物状态
    void clearMonsters();
    // 按当前相机位置渲染所有怪物
    void renderMonsters(SDL_Renderer* renderer, const Camera& camera, Uint32 worldTime);
    // 查找距离玩家最近的存活怪物
    int findNearestAliveMonster(int playerX, int playerY, int maxDistance) const;
    // 读取指定怪物信息
    bool getMonster(int monsterId, MonsterInfo& monster) const;
    // 返回怪物阻挡盒
    std::vector<SDL_Rect> collisionRects() const;

private:
    MonsterManager() = default;
    MonsterManager(const MonsterManager&) = delete;
    MonsterManager& operator=(const MonsterManager&) = delete;
    // 加载并缓存指定路径的怪物纹理
    SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& spritePath);
    // 为指定怪物准备渲染状态。
    void ensureRenderState(const MonsterInfo& monster);
    // 从 XML 构建怪物部件模板。
    std::vector<PartBase> buildPartTemplate(const std::string& spritePath) const;
    // 返回怪物附加部件路径。
    std::vector<std::string> accessorySpritePaths(const std::string& spritePath) const;
    // 计算怪物当前整体包围盒。
    SDL_Rect renderBounds(const std::vector<PartBase>& parts,
                          const CharaAction& actionName,
                          const CharaDirection& direction,
                          int drawX,
                          int drawY,
                          float scale) const;

    struct MonsterRenderState
    {
        std::vector<PartBase> parts;
        CharaAction action = CharaAction::STAND;
        CharaDirection direction = CharaDirection::DOWN;
        int lastX = 0;
        int lastY = 0;
        bool initialized = false;
        std::string spritePath;
    };

    struct TextureCache
    {
        SDL_Texture* texture = nullptr;
        int width = 0;
        int height = 0;
    };

private:
    // 以怪物 ID 为键的怪物状态表
    std::map<int, MonsterInfo> m_monsterMap;
    // 每只怪物独立的动画渲染状态
    std::map<int, MonsterRenderState> m_renderStates;
    // 以 XML 路径为键的部件模板缓存
    std::map<std::string, std::vector<PartBase>> m_partTemplates;
    // 以图片路径为键的怪物纹理缓存表
    std::map<std::string, TextureCache> m_textureCache;
    // 上次推进怪物动画的时间戳
    Uint32 m_lastRenderTime = 0;
};
