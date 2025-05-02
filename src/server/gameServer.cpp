#include "gameServer.hpp"

#include <functional>
#include "json.hpp"
#include "gameService.hpp"
#include "serverComonFunc.hpp"
#include <spdlog/spdlog.h>


GameServer::GameServer(EventLoop *loop, const InetAddress &listenAddr, const string &nameArg)
    :m_server(loop, listenAddr, nameArg)
    ,m_loop(loop)
{
    // 注册连接回调
    m_server.setConnectionCallback(std::bind(&GameServer::onConnection, this,  _1));
    
    // 注册消息回调
    m_server.setMessageCallback(std::bind(&GameServer::onMessage, this, _1, _2, _3));

    // 设置线程数
    m_server.setThreadNum(4);


}

void GameServer::start()
{
    m_server.start();
}

void GameServer::onConnection(const TcpConnectionPtr &conn)
{
    GameService::getInstance().clientConnection(conn);
}

void GameServer::onMessage(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp time)
{
    std::string buf = buffer->retrieveAllAsString();
    // spdlog::info("收到客户端消息：" + buf);
    uint32_t msgId;
    std::string payload;
    if(unpacker.unpack(buf, msgId, payload))
    {
        MsgHander msgHandler =  GameService::getInstance().getMsgHander(msgId);
        // spdlog::info("解包后消息" + payload);
        
        json js = json::parse(payload);
        msgHandler(conn, js, time);
    }

    
}

