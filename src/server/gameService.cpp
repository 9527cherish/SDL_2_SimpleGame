#include "gameService.hpp"
#include <spdlog/spdlog.h>

GameService::GameService()
{
    registerHandlers();
}

MsgHander GameService::getMsgHander(int msgId) const
{
    return m_router.resolve(msgId);
}

void GameService::clientConnection(const TcpConnectionPtr& conn)
{
    if (!conn->connected())
    {
        const std::string playerUuid = m_connections.removeConnection(conn);
        if (!playerUuid.empty())
        {
            m_players.removeByUuid(playerUuid);

            PlayerInfo playerInfo;
            playerInfo.uuid = playerUuid;
            m_connections.broadcast(ENUM_MSG_DELETE_PLAYER_RESPONSE, playerInfo);
        }

        conn->shutdown();
        spdlog::info("{} 断开连接！", conn->peerAddress().toIpPort());
        return;
    }

    m_connections.addConnection(conn);
    spdlog::info("接收到来自 {} 的请求", conn->peerAddress().toIpPort());
}

void GameService::reset()
{
}

void GameService::registerHandlers()
{
    m_router.registerHandler(ENUM_MSG_REGISTER_UPDATE_PLAYER_REQUEST,
                             std::bind(&GameService::dealRegisterUpdatePlayer, this, _1, _2, _3));
    m_router.registerHandler(ENUM_MSG_DELETE_PLAYER_REQUEST,
                             std::bind(&GameService::dealDeletePlayer, this, _1, _2, _3));
    m_router.registerHandler(ENUM_MSG_SYNC_PLAYERS_REQUEST,
                             std::bind(&GameService::dealSyncPlayers, this, _1, _2, _3));
    m_router.registerHandler(ENUM_MSG_SYNC_TREES_REQUEST,
                             std::bind(&GameService::dealSyncTrees, this, _1, _2, _3));
    m_router.registerHandler(ENUM_MSG_HIT_TREE_REQUEST,
                             std::bind(&GameService::dealHitTree, this, _1, _2, _3));
    m_router.registerHandler(ENUM_MSG_SENDMESSAGE_REQUEST,
                             std::bind(&GameService::dealSendMessage, this, _1, _2, _3));
}

void GameService::dealRegisterUpdatePlayer(const TcpConnectionPtr& conn, json js, Timestamp time)
{
    (void)time;

    broadcastTrees(m_trees.refreshTrees());

    const PlayerInfo player = m_players.parsePlayer(js);
    m_players.upsert(player);
    m_connections.bindPlayer(conn, player.uuid);

    spdlog::info("更新人物: {}", player.uuid);
    m_connections.broadcast(ENUM_MSG_REGISTER_UPDATE_PLAYER_RESPONSE, player);
}

void GameService::dealDeletePlayer(const TcpConnectionPtr& conn, json js, Timestamp time)
{
    (void)time;

    const PlayerInfo player = m_players.parsePlayer(js);
    m_players.removeByUuid(player.uuid);
    m_connections.unbindPlayer(conn);

    spdlog::info("删除人物: {}", player.uuid);
    m_connections.broadcast(ENUM_MSG_DELETE_PLAYER_RESPONSE, player);
}

void GameService::dealSyncPlayers(const TcpConnectionPtr& conn, json js, Timestamp time)
{
    (void)js;
    (void)time;

    json response;
    response["players"] = m_players.allPlayers();
    m_connections.sendTo(conn, ENUM_MSG_SYNC_PLAYERS_RESPONSE, response);
}

void GameService::dealSyncTrees(const TcpConnectionPtr& conn, json js, Timestamp time)
{
    (void)js;
    (void)time;

    broadcastTrees(m_trees.refreshTrees());

    json response;
    response["trees"] = m_trees.getAllTrees();
    m_connections.sendTo(conn, ENUM_MSG_SYNC_TREES_RESPONSE, response);
}

void GameService::dealHitTree(const TcpConnectionPtr& conn, json js, Timestamp time)
{
    (void)conn;
    (void)time;

    broadcastTrees(m_trees.refreshTrees());

    const TreeHitRequest request = js.get<TreeHitRequest>();
    TreeInfo treeInfo;
    if (!m_trees.hitTree(request, treeInfo)) {
        return;
    }

    m_connections.broadcast(ENUM_MSG_HIT_TREE_RESPONSE, treeInfo);
}

void GameService::broadcastTrees(const std::vector<TreeInfo>& trees)
{
    for (const TreeInfo& tree : trees)
    {
        m_connections.broadcast(ENUM_MSG_UPDATE_TREE_PUSH, tree);
    }
}

void GameService::dealSendMessage(const TcpConnectionPtr& conn, json js, Timestamp time)
{
    (void)conn;
    (void)js;
    (void)time;
}
