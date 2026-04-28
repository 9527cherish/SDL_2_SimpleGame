#pragma once

#include <SDL2/SDL.h>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "camera.hpp"
#include "messageInfo.hpp"

class TreeManager
{
public:
    ~TreeManager();
    static TreeManager& getInstance();

    void syncTrees(const std::vector<TreeInfo>& trees);
    void updateTree(const TreeInfo& treeInfo);
    void clearTrees();
    void renderTrees(SDL_Renderer* renderer, const Camera& camera);
    int findNearestAliveTree(int playerX, int playerY, int maxDistance) const;

private:
    TreeManager() = default;
    SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& spritePath);

    struct TextureCache
    {
        SDL_Texture* texture = nullptr;
        int width = 0;
        int height = 0;
    };

private:
    std::map<int, TreeInfo> m_treeMap;
    std::map<std::string, TextureCache> m_textureCache;
};
