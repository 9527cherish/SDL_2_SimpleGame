
#pragma once 

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <mutex>
#include <set>

using namespace muduo;
using namespace muduo::net;

class GameServer{

public:
    GameServer(EventLoop* loop,
            const InetAddress& listenAddr,
            const string& nameArg
    );

    void start();

private:
    void onConnection(const TcpConnectionPtr &);
    void onMessage(const TcpConnectionPtr &, Buffer*, Timestamp);

    TcpServer m_server;
    EventLoop* m_loop;
};
