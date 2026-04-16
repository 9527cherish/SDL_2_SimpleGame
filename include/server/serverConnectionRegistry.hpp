#pragma once

#include <map>
#include <mutex>
#include <set>
#include <string>
#include <muduo/net/TcpConnection.h>
#include "messageInfo.hpp"

using namespace muduo;
using namespace muduo::net;

// ServerConnectionRegistry 负责维护在线连接和连接到玩家 UUID 的映射关系
class ServerConnectionRegistry
{
public:
    // 注册一个新连接。
    void addConnection(const TcpConnectionPtr& conn);
    // 移除连接并返回其绑定的玩家 UUID。
    std::string removeConnection(const TcpConnectionPtr& conn);
    // 将连接和玩家 UUID 绑定起来。
    void bindPlayer(const TcpConnectionPtr& conn, const std::string& playerUuid);
    // 解除连接和玩家 UUID 的绑定。
    void unbindPlayer(const TcpConnectionPtr& conn);

    // 直接广播原始字符串消息。
    void broadcastRaw(const std::string& message);
    // 广播结构化业务消息。
    void broadcast(ENUM_MSG_TYPE msgType, const json& data);
    // 向指定连接单播业务消息。
    void sendTo(const TcpConnectionPtr& conn, ENUM_MSG_TYPE msgType, const json& data);

private:
    // 当前所有在线连接集合
    std::set<TcpConnectionPtr> m_connections;
    // 连接名到玩家 UUID 的映射表
    std::map<std::string, std::string> m_connPlayerMap;
    // 保护连接表与映射表的互斥锁
    std::mutex m_mutex;
};
