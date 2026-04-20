#pragma once

#include <SDL2/SDL.h>
#include <map>
#include <string>
#include <vector>
#include "camera.hpp"
#include "messageInfo.hpp"

// DropManager 管理客户端掉落物快照、纹理缓存与场景渲染
class DropManager
{
public:
    ~DropManager();
    // 返回掉落物管理器单例
    static DropManager& getInstance();

    // 用服务器掉落物快照整体替换当前客户端状态
    void syncDrops(const std::vector<DropInfo>& drops);
    // 更新单个掉落物状态
    void updateDrop(const DropInfo& drop);
    // 清空当前掉落物状态
    void clearDrops();
    // 按当前相机位置渲染所有未拾取掉落物
    void renderDrops(SDL_Renderer* renderer, const Camera& camera, Uint32 worldTime);
    // 查找距离玩家最近的可拾取掉落物
    int findNearestAvailableDrop(int playerX, int playerY, int maxDistance) const;

private:
    DropManager() = default;
    DropManager(const DropManager&) = delete;
    DropManager& operator=(const DropManager&) = delete;
    // 加载并缓存指定路径的掉落物纹理
    SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& spritePath);

    struct TextureCache
    {
        SDL_Texture* texture = nullptr;
        int width = 0;
        int height = 0;
    };

private:
    // 以掉落物 ID 为键的掉落物状态表
    std::map<int, DropInfo> m_dropMap;
    // 以图片路径为键的掉落物纹理缓存表
    std::map<std::string, TextureCache> m_textureCache;
};
