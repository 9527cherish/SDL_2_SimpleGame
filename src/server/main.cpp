
#include "gameServer.hpp"
#include "serverComonFunc.hpp"

// 服务端程序入口：初始化日志并启动 Muduo 事件循环。
int main(){

    ServerComonFunc::getInstance();
    EventLoop loop;
    InetAddress addr("127.0.0.1", 8080);
    GameServer server(&loop, addr, "chatserver");
    server.start();
    loop.loop();
    return 0;
}
