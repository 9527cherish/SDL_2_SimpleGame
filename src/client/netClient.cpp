#include "netClient.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sstream>
#include <uuid/uuid.h>
#include <spdlog/spdlog.h>

#include "characterAction.hpp"
#include "characterDirection.hpp"
#include "dataManager.hpp"
#include "persona.hpp"

NetClient::NetClient()
    : m_socketFd(-1)
    , m_serverIp("127.0.0.1")
    , m_serverPort(8080)
    , m_running(false)
    , m_enteredGame(false)
{
}

NetClient::~NetClient()
{
    leaveGame();
}

NetClient& NetClient::getInstance()
{
    static NetClient instance;
    return instance;
}

bool NetClient::enterGame()
{
    if (m_localUuid.empty()) {
        m_localUuid = generateUuid();
    }

    if (!connectServer()) {
        return false;
    }

    DataManager::getInstance().clearRemotePersonas();
    m_enteredGame = true;
    sendMessage(ENUM_MSG_SYNC_PLAYERS_REQUEST, json{{"uuid", m_localUuid}});
    syncCurrentPlayer();
    return true;
}

void NetClient::leaveGame()
{
    if (m_enteredGame && m_socketFd >= 0 && !m_localUuid.empty())
    {
        PlayerInfo playerInfo;
        playerInfo.uuid = m_localUuid;
        sendMessage(ENUM_MSG_DELETE_PLAYER_REQUEST, playerInfo);
    }

    m_enteredGame = false;
    closeConnection();
    DataManager::getInstance().clearRemotePersonas();
}

void NetClient::syncCurrentPlayer()
{
    if (!m_enteredGame || m_socketFd < 0) {
        return;
    }

    PlayerInfo playerInfo = buildCurrentPlayerInfo();
    if (playerInfo.uuid.empty() || playerInfo.personaId < 0) {
        return;
    }

    sendMessage(ENUM_MSG_REGISTER_UPDATE_PLAYER_REQUEST, playerInfo);
}

bool NetClient::connectServer()
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
    serverAddr.sin_port = htons(m_serverPort);
    serverAddr.sin_addr.s_addr = inet_addr(m_serverIp.c_str());

    if (::connect(m_socketFd, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0)
    {
        spdlog::error("连接服务器失败 {}:{} - {}", m_serverIp, m_serverPort, std::strerror(errno));
        closeConnection();
        return false;
    }

    m_running = true;
    m_receiveThread = std::thread(&NetClient::receiveLoop, this);
    spdlog::info("客户端已连接到服务器 {}:{}", m_serverIp, m_serverPort);
    return true;
}

void NetClient::closeConnection()
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

void NetClient::receiveLoop()
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
            std::string line = pendingMessage.substr(0, pos);
            pendingMessage.erase(0, pos + 1);

            if (!line.empty())
            {
                json packet = json::parse(line, nullptr, false);
                if (packet.is_discarded()) {
                    spdlog::error("收到无法解析的消息: {}", line);
                } else {
                    handleMessage(packet.value("msgId", -1), packet.value("data", json::object()));
                }
            }

            pos = pendingMessage.find('\n');
        }
    }

    m_running = false;
}

void NetClient::handleMessage(int msgId, const json& data)
{
    switch (msgId)
    {
    case ENUM_MSG_REGISTER_UPDATE_PLAYER_RESPONSE:
    {
        PlayerInfo playerInfo = data.get<PlayerInfo>();
        if (playerInfo.uuid == m_localUuid) {
            return;
        }
        DataManager::getInstance().syncRemotePersona(playerInfo);
        break;
    }
    case ENUM_MSG_DELETE_PLAYER_RESPONSE:
    {
        PlayerInfo playerInfo = data.get<PlayerInfo>();
        if (playerInfo.uuid == m_localUuid) {
            return;
        }
        DataManager::getInstance().deleteRemotePersona(playerInfo.uuid);
        break;
    }
    case ENUM_MSG_SYNC_PLAYERS_RESPONSE:
    {
        DataManager::getInstance().clearRemotePersonas();
        std::vector<PlayerInfo> players = data.value("players", std::vector<PlayerInfo>());
        for (const PlayerInfo& playerInfo : players)
        {
            if (playerInfo.uuid == m_localUuid) {
                continue;
            }
            DataManager::getInstance().syncRemotePersona(playerInfo);
        }
        break;
    }
    default:
        break;
    }
}

void NetClient::sendMessage(ENUM_MSG_TYPE msgType, const json& data)
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

PlayerInfo NetClient::buildCurrentPlayerInfo()
{
    PlayerInfo playerInfo;
    std::shared_ptr<Persona> persona = DataManager::getInstance().currentPersona();
    if (persona == nullptr) {
        return playerInfo;
    }

    playerInfo.uuid = m_localUuid;
    playerInfo.personaId = DataManager::getInstance().currentPersonaIndex();
    playerInfo.name = "player_" + m_localUuid.substr(0, 8);
    playerInfo.x = persona->x();
    playerInfo.y = persona->y();
    playerInfo.action = ActionMapper::to_string(persona->action());
    playerInfo.direction = DirectionMapper::to_string(persona->direction());
    return playerInfo;
}

std::string NetClient::generateUuid()
{
    uuid_t uuidValue;
    uuid_generate(uuidValue);
    char uuidStr[37] = {0};
    uuid_unparse(uuidValue, uuidStr);
    return uuidStr;
}
