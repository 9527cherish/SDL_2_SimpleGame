#include "serverConnectionRegistry.hpp"

void ServerConnectionRegistry::addConnection(const TcpConnectionPtr& conn)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_connections.insert(conn);
}

std::string ServerConnectionRegistry::removeConnection(const TcpConnectionPtr& conn)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::string playerUuid;

    auto playerIter = m_connPlayerMap.find(conn->name());
    if (playerIter != m_connPlayerMap.end())
    {
        playerUuid = playerIter->second;
        m_connPlayerMap.erase(playerIter);
    }

    m_connections.erase(conn);
    return playerUuid;
}

void ServerConnectionRegistry::bindPlayer(const TcpConnectionPtr& conn, const std::string& playerUuid)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_connPlayerMap[conn->name()] = playerUuid;
}

void ServerConnectionRegistry::unbindPlayer(const TcpConnectionPtr& conn)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto iter = m_connPlayerMap.find(conn->name());
    if (iter != m_connPlayerMap.end()) {
        m_connPlayerMap.erase(iter);
    }
}

void ServerConnectionRegistry::broadcastRaw(const std::string& message)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (const auto& client : m_connections)
    {
        client->send(message);
    }
}

void ServerConnectionRegistry::broadcast(ENUM_MSG_TYPE msgType, const json& data)
{
    broadcastRaw(packMessage(msgType, data));
}

void ServerConnectionRegistry::sendTo(const TcpConnectionPtr& conn, ENUM_MSG_TYPE msgType, const json& data)
{
    if (conn == nullptr) {
        return;
    }

    conn->send(packMessage(msgType, data));
}
