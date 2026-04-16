#include "Network/clientTransport.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <spdlog/spdlog.h>
#include "messageInfo.hpp"

ClientTransport::ClientTransport()
    : m_socketFd(-1)
    , m_running(false)
{
}

ClientTransport::~ClientTransport()
{
    disconnect();
}

bool ClientTransport::connectTo(const std::string& serverIp, uint16_t serverPort, PacketHandler packetHandler)
{
    if (m_socketFd >= 0) {
        return true;
    }

    m_socketFd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (m_socketFd < 0) {
        spdlog::error("创建客户端 socket 失败: {}", std::strerror(errno));
        return false;
    }

    sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = inet_addr(serverIp.c_str());

    if (::connect(m_socketFd, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0)
    {
        spdlog::error("连接服务器失败 {}:{} - {}", serverIp, serverPort, std::strerror(errno));
        disconnect();
        return false;
    }

    m_packetHandler = std::move(packetHandler);
    m_running = true;
    m_receiveThread = std::thread(&ClientTransport::receiveLoop, this);
    spdlog::info("客户端已连接到服务器 {}:{}", serverIp, serverPort);
    return true;
}

void ClientTransport::disconnect()
{
    const int socketFd = m_socketFd;
    m_socketFd = -1;
    m_running = false;

    if (socketFd >= 0) {
        ::shutdown(socketFd, SHUT_RDWR);
        ::close(socketFd);
    }

    if (m_receiveThread.joinable()) {
        m_receiveThread.join();
    }
}

void ClientTransport::sendPacket(ENUM_MSG_TYPE msgType, const json& data)
{
    if (m_socketFd < 0) {
        return;
    }

    const std::string packet = packMessage(msgType, data);
    std::lock_guard<std::mutex> lock(m_sendMutex);

    ssize_t sent = ::send(m_socketFd, packet.c_str(), packet.size(), 0);
    if (sent < 0) {
        spdlog::error("发送消息失败: {}", std::strerror(errno));
    }
}

bool ClientTransport::isConnected() const
{
    return m_socketFd >= 0;
}

void ClientTransport::receiveLoop()
{
    std::string pendingMessage;
    char buffer[4096] = {0};

    while (m_running)
    {
        ssize_t received = ::recv(m_socketFd, buffer, sizeof(buffer), 0);
        if (received <= 0)
        {
            if (received < 0 && m_running) {
                spdlog::error("接收服务器消息失败: {}", std::strerror(errno));
            }
            break;
        }

        pendingMessage.append(buffer, static_cast<size_t>(received));

        size_t pos = pendingMessage.find('\n');
        while (pos != std::string::npos)
        {
            const std::string line = pendingMessage.substr(0, pos);
            pendingMessage.erase(0, pos + 1);

            if (!line.empty() && m_packetHandler)
            {
                PacketEnvelope packet;
                if (!parsePacketLine(line, packet)) {
                    spdlog::error("收到无法解析的消息: {}", line);
                } else {
                    m_packetHandler(packet.msgId, packet.data);
                }
            }

            pos = pendingMessage.find('\n');
        }
    }

    m_running = false;
}
