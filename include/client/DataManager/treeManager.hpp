#pragma once

#include <SDL2/SDL.h>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "camera.hpp"
#include "messageInfo.hpp"

// TreeManager 管理客户端树木快照、纹理缓存与场景渲染
class TreeManager
{
public:
    ~TreeManager();
    // TreeManager 负责客户端树木快照与渲染缓存。
    static TreeManager& getInstance();

    // 用服务器树木快照整体替换当前客户端状态。
    void syncTrees(const std::vector<TreeInfo>& trees);
    // 更新单棵树的状态。
    void updateTree(const TreeInfo& treeInfo);
    // 清空当前树木状态。
    void clearTrees();
    // 按当前相机位置渲染所有树木。
    void renderTrees(SDL_Renderer* renderer, const Camera& camera);
    // 查找距离玩家最近的存活树木。
    int findNearestAliveTree(int playerX, int playerY, int maxDistance) const;

private:
    TreeManager() = default;
    TreeManager(const TreeManager&) = delete;
    TreeManager& operator=(const TreeManager&) = delete;
    // 加载并缓存指定路径的树木纹理。
    SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& spritePath);

    struct TextureCache
    {
        // 已加载的树木纹理
        SDL_Texture* texture = nullptr;
        // 纹理宽度
        int width = 0;
        // 纹理高度
        int height = 0;
    };

private:
    // 以树 ID 为键的树木状态表
    std::map<int, TreeInfo> m_treeMap;
    // 以图片路径为键的纹理缓存表
    std::map<std::string, TextureCache> m_textureCache;
};
