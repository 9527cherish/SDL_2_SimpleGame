#include "gameService.hpp"
#include "messageInfo.hpp"
#include "serverComonFunc.hpp"
#include <spdlog/spdlog.h>
#include "characterManager.hpp"
#include "messageInfo.hpp"

GameService::GameService()
{
    m_mapMsgHander[ENUM_MSG_REGISTER_UPDATE_PLAYER_REQUEST] = std::bind(&GameService::dealRegisterUpdatePlayer, this, _1, _2, _3);
    m_mapMsgHander[ENUM_MSG_DELETE_PLAYER_REQUEST] = std::bind(&GameService::dealDeletePlayer, this, _1, _2, _3);
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
    std::lock_guard<std::mutex> lock(m_csconnection);
    if(!conn->connected())
    {
        m_connections.erase(conn);
        conn->shutdown();
        spdlog::info(conn->peerAddress().toIpPort() + "断开连接！");
    }
    else
    {
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

void GameService::dealRegisterUpdatePlayer(const TcpConnectionPtr &conn, json js, Timestamp time)
{
    (void)conn; 
    (void)time; 
    PlayerInfo player = CharacterManager::getInstanse().generatePlayer(js);
    CharacterManager::getInstanse().addPlayer(player);
    // spdlog::info("开始广播数据:" + player.uuid);

    auto data = MessagePacker::pack(ENUM_MSG_REGISTER_UPDATE_PLAYER_RESPONSE, js.dump());
    std::string sendMsg(data.data(), data.size());
    
    brodcastMsg(sendMsg);
}

void GameService::dealDeletePlayer(const TcpConnectionPtr &conn, json js, Timestamp time)
{
    (void)conn; 
    (void)time; 
    PlayerInfo player = CharacterManager::getInstanse().generatePlayer(js);
    CharacterManager::getInstanse().deletePlayer(player);
    spdlog::info("删除人物:" + player.uuid);
    auto data = MessagePacker::pack(ENUM_MSG_DELETE_PLAYER_RESPONSE, js.dump());
    std::string sendMsg(data.data(), data.size());
    
    brodcastMsg(sendMsg);
}

void GameService::dealSendMessage(const TcpConnectionPtr &conn, json js, Timestamp time)
{
    (void)conn; 
    (void)time;
    auto data = MessagePacker::pack(ENUM_MSG_SENDMESSAGE_RESPONSE, js.dump());

    spdlog::info("广播聊天消息:" + js.dump());
    std::string sendMsg(data.data(), data.size());

    brodcastMsg(sendMsg);
}
