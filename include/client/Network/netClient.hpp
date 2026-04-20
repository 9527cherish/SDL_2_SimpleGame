#pragma once

#include <string>
#include "messageInfo.hpp"
#include "Network/clientTransport.hpp"

// NetClient 负责客户端联机会话流程和消息级业务同步
class NetClient
{
public:
    ~NetClient();

    // NetClient 是联机会话门面：外部只通过它触发进房、同步和交互请求。
    static NetClient& getInstance();

    // 建立连接并向服务端注册当前玩家。
    bool enterGame();
    // 退出联机会话并通知服务端删除当前玩家。
    void leaveGame();
    // 把本机玩家的最新状态同步给服务端。
    void syncCurrentPlayer();
    // 向服务端发送一次砍树请求。
    void hitTree(int treeId, int damage = 1);
    // 向服务端发送一次攻击怪物请求。
    void hitMonster(int monsterId, int damage = 1, const std::string& weaponType = "");
    // 向服务端发送一次拾取掉落物请求。
    void pickDrop(int dropId);
    // 向服务端请求同步当前背包。
    void syncInventory();
    // 向服务端请求同步当前投射物。
    void syncProjectiles();

private:
    NetClient();
    NetClient(const NetClient&) = delete;
    NetClient& operator=(const NetClient&) = delete;

    // 处理服务端发回的业务消息。
    void handleMessage(int msgId, const json& data);
    // 发送一条业务消息到服务端。
    void sendMessage(ENUM_MSG_TYPE msgType, const json& data);
    // 组装当前本机玩家的同步数据。
    PlayerInfo buildCurrentPlayerInfo();
    // 生成当前客户端唯一 UUID。
    std::string generateUuid();

private:
    // 服务端 IP 地址
    std::string m_serverIp;
    // 服务端端口号
    uint16_t m_serverPort;
    // 当前客户端对应的玩家 UUID
    std::string m_localUuid;
    // 当前是否已经完成进房流程
    bool m_enteredGame;
    // 底层网络传输对象
    ClientTransport m_transport;
};
