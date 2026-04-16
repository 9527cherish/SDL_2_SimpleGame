#pragma once

#include <functional>
#include <map>
#include "json.hpp"
#include <muduo/base/Timestamp.h>
#include <muduo/net/TcpConnection.h>

using namespace muduo;
using namespace muduo::net;
using json = nlohmann::json;

using MsgHander = std::function<void(const TcpConnectionPtr& conn, json js, Timestamp time)>;

// ServerMessageRouter 负责按消息号注册和解析处理函数
class ServerMessageRouter
{
public:
    // 注册一个消息号对应的处理器。
    void registerHandler(int msgId, MsgHander handler);
    // 解析指定消息号对应的处理器。
    MsgHander resolve(int msgId) const;

private:
    // 消息号到处理函数的映射表
    std::map<int, MsgHander> m_handlers;
};
