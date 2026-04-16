#pragma once

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
#include "messageInfo.hpp"

// ServerPlayerStore 负责维护服务端视角下的在线玩家状态
class ServerPlayerStore
{
public:
    // 新增或更新一个玩家状态。
    void upsert(const PlayerInfo& player);
    // 按 UUID 删除玩家状态。
    void removeByUuid(const std::string& uuid);
    // 把 JSON 数据解析成玩家结构。
    PlayerInfo parsePlayer(const json& js) const;
    // 返回所有在线玩家的快照。
    std::vector<PlayerInfo> allPlayers() const;

private:
    // 保护玩家表的互斥锁
    mutable std::mutex m_mutex;
    // 以 UUID 为键的在线玩家表
    std::unordered_map<std::string, PlayerInfo> m_players;
};
