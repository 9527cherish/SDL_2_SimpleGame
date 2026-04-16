
#pragma once 

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include "gameService.hpp"

using namespace muduo;
using namespace muduo::net;

// GameServer 是 Muduo 服务端入口，负责监听连接与接收消息
class GameServer{

public:
    GameServer(EventLoop* loop,
            const InetAddress& listenAddr,
            const string& nameArg
    );

    // 启动 TCP 服务监听。
    void start();

private:
    // 处理连接建立和断开事件。
    void onConnection(const TcpConnectionPtr &);
    // 处理客户端发来的原始消息。
    void onMessage(const TcpConnectionPtr &, Buffer*, Timestamp);

    // Muduo TCP 服务对象
    TcpServer m_server;
    // Muduo 事件循环
    EventLoop* m_loop;
    // 具体的业务处理服务
    GameService m_gameService;
};
