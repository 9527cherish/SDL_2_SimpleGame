#pragma once

#include <chrono>
#include <mutex>
#include <unordered_map>
#include <vector>
#include "messageInfo.hpp"

// ServerTreeManager 负责服务端树木状态、受击和重生逻辑
class ServerTreeManager
{
public:
    // 返回所有树木的当前快照。
    std::vector<TreeInfo> getAllTrees();
    // 对指定树木应用一次伤害。
    bool hitTree(const TreeHitRequest& request, TreeInfo& updatedTree);
    // 刷新应当重生的树木并返回变化列表。
    std::vector<TreeInfo> refreshTrees();

private:
    friend class GameService;
    ServerTreeManager();

    struct TreeRuntime
    {
        // 当前树木的业务状态
        TreeInfo info;
        // 当前树木预计重生的时间点
        std::chrono::steady_clock::time_point respawnTime;
    };

private:
    // 保护树木状态的互斥锁
    std::mutex m_treeMutex;
    // 以树 ID 为键的树木运行时状态表
    std::unordered_map<int, TreeRuntime> m_treeMap;
};
