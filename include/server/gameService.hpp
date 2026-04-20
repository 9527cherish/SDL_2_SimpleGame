#pragma once

#include "serverConnectionRegistry.hpp"
#include "serverMessageRouter.hpp"
#include "serverMonsterManager.hpp"
#include "serverPlayerStore.hpp"
#include "serverTreeManager.hpp"

// GameService 负责服务端消息分发、玩家同步和树木同步编排
class GameService
{
public:
    GameService();

    // 根据消息号解析对应的业务处理函数。
    MsgHander getMsgHander(int msgId) const;
    // 处理客户端连接生命周期事件。
    void clientConnection(const TcpConnectionPtr& conn);
    // 处理服务重置时的状态清理。
    void reset();

private:
    // 注册所有支持的消息处理函数。
    void registerHandlers();
    // 处理玩家注册或状态更新请求。
    void dealRegisterUpdatePlayer(const TcpConnectionPtr& conn, json js, Timestamp time);
    // 处理玩家离线删除请求。
    void dealDeletePlayer(const TcpConnectionPtr& conn, json js, Timestamp time);
    // 处理聊天消息请求。
    void dealSendMessage(const TcpConnectionPtr& conn, json js, Timestamp time);
    // 处理玩家列表同步请求。
    void dealSyncPlayers(const TcpConnectionPtr& conn, json js, Timestamp time);
    // 处理树木列表同步请求。
    void dealSyncTrees(const TcpConnectionPtr& conn, json js, Timestamp time);
    // 处理砍树请求。
    void dealHitTree(const TcpConnectionPtr& conn, json js, Timestamp time);
    // 处理怪物列表同步请求。
    void dealSyncMonsters(const TcpConnectionPtr& conn, json js, Timestamp time);
    // 处理攻击怪物请求。
    void dealHitMonster(const TcpConnectionPtr& conn, json js, Timestamp time);
    // 处理掉落物列表同步请求。
    void dealSyncDrops(const TcpConnectionPtr& conn, json js, Timestamp time);
    // 处理拾取掉落物请求。
    void dealPickDrop(const TcpConnectionPtr& conn, json js, Timestamp time);
    // 处理背包同步请求。
    void dealSyncInventory(const TcpConnectionPtr& conn, json js, Timestamp time);
    // 处理投射物同步请求。
    void dealSyncProjectiles(const TcpConnectionPtr& conn, json js, Timestamp time);
    // 向所有客户端广播树木状态更新。
    void broadcastTrees(const std::vector<TreeInfo>& trees);
    // 向所有客户端广播怪物状态更新。
    void broadcastMonsters(const std::vector<MonsterInfo>& monsters);
    // 向所有客户端广播掉落物状态更新。
    void broadcastDrops(const std::vector<DropInfo>& drops);
    // 向所有客户端广播投射物状态更新。
    void broadcastProjectiles(const std::vector<ProjectileInfo>& projectiles);
    // 向指定客户端发送背包状态。
    void sendInventory(const TcpConnectionPtr& conn, const std::string& playerUuid);

private:
    // 服务端消息路由表
    ServerMessageRouter m_router;
    // 在线连接与玩家映射表
    ServerConnectionRegistry m_connections;
    // 在线玩家状态仓库
    ServerPlayerStore m_players;
    // 怪物与掉落物领域状态管理器
    ServerMonsterManager m_monsters;
    // 树木领域状态管理器
    ServerTreeManager m_trees;
};
