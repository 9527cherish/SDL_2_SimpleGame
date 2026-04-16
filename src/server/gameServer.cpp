#include "gameServer.hpp"

#include <functional>
#include "messageInfo.hpp"
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
    m_gameService.clientConnection(conn);
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
            PacketEnvelope packet;
            if (!parsePacketLine(line, packet))
            {
                spdlog::error("收到无法解析的客户端消息: {}", line);
            }
            else
            {
                MsgHander msgHandler = m_gameService.getMsgHander(packet.msgId);
                msgHandler(conn, packet.data, time);
            }
        }

        eol = buffer->findEOL();
    }
}
