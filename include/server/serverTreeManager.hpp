#pragma once

#include <chrono>
#include <mutex>
#include <unordered_map>
#include <vector>
#include "messageInfo.hpp"

class ServerTreeManager
{
public:
    static ServerTreeManager& getInstance();

    std::vector<TreeInfo> getAllTrees();
    bool hitTree(const TreeHitRequest& request, TreeInfo& updatedTree);
    std::vector<TreeInfo> refreshTrees();

private:
    ServerTreeManager();

    struct TreeRuntime
    {
        TreeInfo info;
        std::chrono::steady_clock::time_point respawnTime;
    };

private:
    std::mutex m_treeMutex;
    std::unordered_map<int, TreeRuntime> m_treeMap;
};
