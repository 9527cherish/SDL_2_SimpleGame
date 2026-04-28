#include "gameService.hpp"
#include "messageInfo.hpp"
#include "serverComonFunc.hpp"
#include <spdlog/spdlog.h>
#include "characterManager.hpp"
#include "messageInfo.hpp"
#include "serverTreeManager.hpp"

GameService::GameService()
{
    m_mapMsgHander[ENUM_MSG_REGISTER_UPDATE_PLAYER_REQUEST] = std::bind(&GameService::dealRegisterUpdatePlayer, this, _1, _2, _3);
    m_mapMsgHander[ENUM_MSG_DELETE_PLAYER_REQUEST] = std::bind(&GameService::dealDeletePlayer, this, _1, _2, _3);
    m_mapMsgHander[ENUM_MSG_SYNC_PLAYERS_REQUEST] = std::bind(&GameService::dealSyncPlayers, this, _1, _2, _3);
    m_mapMsgHander[ENUM_MSG_SYNC_TREES_REQUEST] = std::bind(&GameService::dealSyncTrees, this, _1, _2, _3);
    m_mapMsgHander[ENUM_MSG_HIT_TREE_REQUEST] = std::bind(&GameService::dealHitTree, this, _1, _2, _3);
    m_mapMsgHander[ENUM_MSG_SENDMESSAGE_REQUEST] = std::bind(&GameService::dealSendMessage, this, _1, _2, _3);
}

GameService &GameService::getInstance()
{
    static GameService instance;
    return instance;
}

MsgHander GameService::getMsgHander(int MsgId)
{
    
    auto iter = m_mapMsgHander.find(MsgId);
    if(iter == m_mapMsgHander.end())
    {
        return [=](const TcpConnectionPtr &, json , Timestamp)
        {
            spdlog::error("msgid" + std::to_string(MsgId) + "找不到对应的handler");
        };
    }
    return m_mapMsgHander[MsgId];
}

void GameService::clientConnection(const TcpConnectionPtr &conn)
{
    std::string deleteMsg;
    if(!conn->connected())
    {
        {
            std::lock_guard<std::mutex> lock(m_csconnection);
            auto playerIter = m_mapConnPlayer.find(conn->name());
            if (playerIter != m_mapConnPlayer.end())
            {
                PlayerInfo playerInfo;
                playerInfo.uuid = playerIter->second;
                CharacterManager::getInstanse().deletePlayer(playerInfo);
                deleteMsg = packMessage(ENUM_MSG_DELETE_PLAYER_RESPONSE, playerInfo);
                m_mapConnPlayer.erase(playerIter);
            }
            m_connections.erase(conn);
        }
        if (!deleteMsg.empty())
        {
            brodcastMsg(deleteMsg);
        }
        conn->shutdown();
        spdlog::info(conn->peerAddress().toIpPort() + "断开连接！");
    }
    else
    {
        std::lock_guard<std::mutex> lock(m_csconnection);
        m_connections.insert(conn);
        spdlog::info("接收到来自" + conn->peerAddress().toIpPort() + "的请求");
    }
}

void GameService::reset()
{
}

void GameService::brodcastMsg(const std::string &msg)
{
    std::lock_guard<std::mutex> lock(m_csconnection);
    for (const auto& client : m_connections) 
    {
        client->send(msg);
    }
}

void GameService::sendMsg(const TcpConnectionPtr &conn, ENUM_MSG_TYPE msgType, const json &js)
{
    if (conn == nullptr) {
        return;
    }

    conn->send(packMessage(msgType, js));
}

void GameService::dealRegisterUpdatePlayer(const TcpConnectionPtr &conn, json js, Timestamp time)
{
    (void)conn; 
    (void)time; 
    broadcastTrees(ServerTreeManager::getInstance().refreshTrees());
    PlayerInfo player = CharacterManager::getInstanse().generatePlayer(js);
    CharacterManager::getInstanse().addPlayer(player);
    {
        std::lock_guard<std::mutex> lock(m_csconnection);
        m_mapConnPlayer[conn->name()] = player.uuid;
    }

    spdlog::info("更新人物: {}", player.uuid);
    brodcastMsg(packMessage(ENUM_MSG_REGISTER_UPDATE_PLAYER_RESPONSE, player));
}

void GameService::dealDeletePlayer(const TcpConnectionPtr &conn, json js, Timestamp time)
{
    (void)conn; 
    (void)time; 
    PlayerInfo player = CharacterManager::getInstanse().generatePlayer(js);
    CharacterManager::getInstanse().deletePlayer(player);
    spdlog::info("删除人物:" + player.uuid);
    {
        std::lock_guard<std::mutex> lock(m_csconnection);
        auto iter = m_mapConnPlayer.find(conn->name());
        if (iter != m_mapConnPlayer.end()) {
            m_mapConnPlayer.erase(iter);
        }
    }
    brodcastMsg(packMessage(ENUM_MSG_DELETE_PLAYER_RESPONSE, player));
}

void GameService::dealSyncPlayers(const TcpConnectionPtr &conn, json js, Timestamp time)
{
    (void)js;
    (void)time;
    json response;
    response["players"] = CharacterManager::getInstanse().getAllPlayers();
    sendMsg(conn, ENUM_MSG_SYNC_PLAYERS_RESPONSE, response);
}

void GameService::dealSyncTrees(const TcpConnectionPtr &conn, json js, Timestamp time)
{
    (void)js;
    (void)time;
    broadcastTrees(ServerTreeManager::getInstance().refreshTrees());
    json response;
    response["trees"] = ServerTreeManager::getInstance().getAllTrees();
    sendMsg(conn, ENUM_MSG_SYNC_TREES_RESPONSE, response);
}

void GameService::dealHitTree(const TcpConnectionPtr &conn, json js, Timestamp time)
{
    (void)conn;
    (void)time;
    broadcastTrees(ServerTreeManager::getInstance().refreshTrees());

    TreeHitRequest request = js.get<TreeHitRequest>();
    TreeInfo treeInfo;
    if (!ServerTreeManager::getInstance().hitTree(request, treeInfo)) {
        return;
    }

    brodcastMsg(packMessage(ENUM_MSG_HIT_TREE_RESPONSE, treeInfo));
}

void GameService::broadcastTrees(const std::vector<TreeInfo>& trees)
{
    for (const TreeInfo& tree : trees)
    {
        brodcastMsg(packMessage(ENUM_MSG_UPDATE_TREE_PUSH, tree));
    }
}

void GameService::dealSendMessage(const TcpConnectionPtr &conn, json js, Timestamp time)
{
    (void)conn; 
    (void)time;
//     auto data = MessagePacker::pack(ENUM_MSG_SENDMESSAGE_RESPONSE, js.dump());

//     spdlog::info("广播聊天消息:" + js.dump());
//     std::string sendMsg(data.data(), data.size());

//     brodcastMsg(sendMsg);
}
