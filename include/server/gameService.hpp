#pragma once 

#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include "json.hpp"
#include <map>
#include <set>
#include <mutex>
#include "messagePacker.hpp"

using namespace muduo;
using namespace muduo::net;
using json = nlohmann::json;


using MsgHander = std::function<void(const TcpConnectionPtr &conn, json js, Timestamp)>;


class GameService
{

public:

    GameService();
    // 获取单例模式
    static GameService& getInstance();
    // 获取消息队形的处理器
    MsgHander getMsgHander(int MsgId);
    // 处理客户端连接
    void clientConnection(const TcpConnectionPtr &conn);
    // 处理服务器异常退出
    void reset();

    void brodcastMsg(const std::string &msg);

    // 处理玩家人物连接时，或者更新时发过来的消息
    void dealRegisterUpdatePlayer(const TcpConnectionPtr &conn, json js, Timestamp time);

    // 处理玩家人物断开连接时发送的消息
    void dealDeletePlayer(const TcpConnectionPtr &conn, json js, Timestamp time);

    // 处理玩家在公共频道发送的消息
    void dealSendMessage(const TcpConnectionPtr &conn, json js, Timestamp time);

private:

    std::map<int, MsgHander> m_mapMsgHander;
    std::map<int, TcpConnectionPtr> m_mapUserConn;
    std::mutex m_mutexUserConn;

    std::set<TcpConnectionPtr> m_connections;
    std::mutex m_csconnection;
};