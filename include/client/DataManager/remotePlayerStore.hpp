#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <vector>
#include "messageInfo.hpp"
#include "remotepersona.hpp"

// RemotePlayerStore 负责缓存并管理所有远端玩家显示实例
class RemotePlayerStore
{
public:
    // 远端玩家仓库只关心网络同步后的显示状态，不参与本机输入。
    static RemotePlayerStore& getInstance();

    // 根据同步消息创建或更新远端玩家。
    void syncRemotePlayer(const PlayerInfo& playerInfo);
    // 按 UUID 删除远端玩家。
    void deleteRemotePlayer(const std::string& uuid);
    // 清空所有远端玩家缓存。
    void clear();
    // 推进远端玩家的逐帧更新。
    void advance(Uint32 deltaTime);
    // 导出当前所有远端玩家实例。
    void getPlayers(std::vector<std::shared_ptr<RemotePersona>>& players);

private:
    RemotePlayerStore() = default;
    RemotePlayerStore(const RemotePlayerStore&) = delete;
    RemotePlayerStore& operator=(const RemotePlayerStore&) = delete;

    struct RemotePersonaData
    {
        // 当前远端玩家使用的人物模板编号
        int personaId = -1;
        // 当前远端玩家的显示实例
        std::shared_ptr<RemotePersona> persona;
        // 最近一次同步到的目标 X 坐标
        int targetX = 0;
        // 最近一次同步到的目标 Y 坐标
        int targetY = 0;
        // 最近一次同步到的目标动作
        CharaAction targetAction = CharaAction::STAND;
        // 最近一次同步到的目标方向
        CharaDirection targetDirection = CharaDirection::DOWN;
    };

private:
    // 以玩家 UUID 为键的远端玩家表
    std::map<std::string, RemotePersonaData> m_remotePersonas;
    // 保护远端玩家表的互斥锁
    std::mutex m_mutex;
};
