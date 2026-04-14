
#include "gameServer.hpp"
#include "gameService.hpp"
#include "serverComonFunc.hpp"


int main(){

    ServerComonFunc::getInstance();
    EventLoop loop;
    InetAddress addr("127.0.0.1", 8080);
    GameServer server(&loop, addr, "chatserver");
    server.start();
    loop.loop();
    return 0;
}