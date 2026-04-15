#include "serverTreeManager.hpp"
#include <algorithm>

namespace {
constexpr int kTreeMaxHp = 5;
constexpr int kTreeRespawnSeconds = 15;
const char* kTreeSpritePath = "../images/tmwa/graphics/sprites/npcs/tree-large.png";
}

ServerTreeManager& ServerTreeManager::getInstance()
{
    static ServerTreeManager instance;
    return instance;
}

ServerTreeManager::ServerTreeManager()
{
    const std::vector<std::pair<int, int>> treePositions{
        {320, 320},
        {520, 260},
        {760, 360},
        {980, 300},
        {1180, 420}
    };

    int treeId = 1;
    for (const auto& [x, y] : treePositions)
    {
        TreeRuntime runtime;
        runtime.info.treeId = treeId++;
        runtime.info.x = x;
        runtime.info.y = y;
        runtime.info.hp = kTreeMaxHp;
        runtime.info.maxHp = kTreeMaxHp;
        runtime.info.state = ENUM_TREE_STATE_ALIVE;
        runtime.info.respawnSeconds = kTreeRespawnSeconds;
        runtime.info.spritePath = kTreeSpritePath;
        runtime.respawnTime = std::chrono::steady_clock::time_point::min();
        m_treeMap[runtime.info.treeId] = runtime;
    }
}

std::vector<TreeInfo> ServerTreeManager::getAllTrees()
{
    refreshTrees();

    std::lock_guard<std::mutex> lock(m_treeMutex);
    std::vector<TreeInfo> trees;
    trees.reserve(m_treeMap.size());
    for (const auto& [treeId, runtime] : m_treeMap)
    {
        (void)treeId;
        trees.emplace_back(runtime.info);
    }
    std::sort(trees.begin(), trees.end(), [](const TreeInfo& lhs, const TreeInfo& rhs) {
        return lhs.treeId < rhs.treeId;
    });
    return trees;
}

bool ServerTreeManager::hitTree(const TreeHitRequest& request, TreeInfo& updatedTree)
{
    refreshTrees();

    std::lock_guard<std::mutex> lock(m_treeMutex);
    auto iter = m_treeMap.find(request.treeId);
    if (iter == m_treeMap.end()) {
        return false;
    }

    TreeRuntime& runtime = iter->second;
    if (runtime.info.state != ENUM_TREE_STATE_ALIVE) {
        updatedTree = runtime.info;
        return false;
    }

    runtime.info.hp = std::max(0, runtime.info.hp - std::max(1, request.damage));
    if (runtime.info.hp == 0)
    {
        runtime.info.state = ENUM_TREE_STATE_STUMP;
        runtime.respawnTime = std::chrono::steady_clock::now() + std::chrono::seconds(runtime.info.respawnSeconds);
    }

    updatedTree = runtime.info;
    return true;
}

std::vector<TreeInfo> ServerTreeManager::refreshTrees()
{
    std::lock_guard<std::mutex> lock(m_treeMutex);
    std::vector<TreeInfo> updatedTrees;
    const auto now = std::chrono::steady_clock::now();

    for (auto& [treeId, runtime] : m_treeMap)
    {
        (void)treeId;
        if (runtime.info.state == ENUM_TREE_STATE_ALIVE) {
            continue;
        }

        if (now < runtime.respawnTime) {
            continue;
        }

        runtime.info.hp = runtime.info.maxHp;
        runtime.info.state = ENUM_TREE_STATE_ALIVE;
        runtime.respawnTime = std::chrono::steady_clock::time_point::min();
        updatedTrees.emplace_back(runtime.info);
    }

    return updatedTrees;
}
