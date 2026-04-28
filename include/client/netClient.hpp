#pragma once

#include <atomic>
#include <mutex>
#include <string>
#include <thread>
#include "messageInfo.hpp"

class NetClient
{
public:
    ~NetClient();

    static NetClient& getInstance();

    bool enterGame();
    void leaveGame();
    void syncCurrentPlayer();
    void hitTree(int treeId, int damage = 1);

private:
    NetClient();

    bool connectServer();
    void closeConnection();
    void receiveLoop();
    void handleMessage(int msgId, const json& data);
    void sendMessage(ENUM_MSG_TYPE msgType, const json& data);
    PlayerInfo buildCurrentPlayerInfo();
    std::string generateUuid();

private:
    int m_socketFd;
    std::string m_serverIp;
    uint16_t m_serverPort;
    std::string m_localUuid;
    std::atomic<bool> m_running;
    std::atomic<bool> m_enteredGame;
    std::thread m_receiveThread;
    std::mutex m_sendMutex;
};
