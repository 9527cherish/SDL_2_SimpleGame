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
    const char* eol = buffer->findEOL();
    while (eol != nullptr)
    {
        std::string line(buffer->peek(), eol);
        buffer->retrieveUntil(eol + 1);

        if (!line.empty())
        {
            json packet = json::parse(line, nullptr, false);
            if (packet.is_discarded())
            {
                spdlog::error("收到无法解析的客户端消息: {}", line);
            }
            else
            {
                const int msgId = packet.value("msgId", -1);
                MsgHander msgHandler = GameService::getInstance().getMsgHander(msgId);
                msgHandler(conn, packet.value("data", json::object()), time);
            }
        }

        eol = buffer->findEOL();
    }
}
