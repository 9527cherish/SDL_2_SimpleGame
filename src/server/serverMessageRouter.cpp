#include "serverMessageRouter.hpp"
#include <spdlog/spdlog.h>

void ServerMessageRouter::registerHandler(int msgId, MsgHander handler)
{
    m_handlers[msgId] = std::move(handler);
}

MsgHander ServerMessageRouter::resolve(int msgId) const
{
    auto iter = m_handlers.find(msgId);
    if (iter == m_handlers.end())
    {
        return [msgId](const TcpConnectionPtr&, json, Timestamp)
        {
            spdlog::error("msgid {} 找不到对应的 handler", msgId);
        };
    }

    return iter->second;
}
