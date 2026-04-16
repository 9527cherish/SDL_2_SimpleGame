#pragma once

#include <atomic>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include "messageInfo.hpp"

// ClientTransport 封装客户端 socket 连接、收包线程和原始发包行为
class ClientTransport
{
public:
    using PacketHandler = std::function<void(int msgId, const json& data)>;

    ClientTransport();
    ~ClientTransport();

    // 连接到指定服务端并启动收包线程。
    bool connectTo(const std::string& serverIp, uint16_t serverPort, PacketHandler packetHandler);
    // 断开当前网络连接并回收线程。
    void disconnect();
    // 发送一条带消息号的 JSON 数据包。
    void sendPacket(ENUM_MSG_TYPE msgType, const json& data);
    // 判断当前是否处于已连接状态。
    bool isConnected() const;

private:
    // 持续接收服务端数据并按行拆包。
    void receiveLoop();

private:
    // 当前连接对应的 socket 文件描述符
    int m_socketFd;
    // 收包线程是否应继续运行
    std::atomic<bool> m_running;
    // 持续读取服务端消息的线程
    std::thread m_receiveThread;
    // 串行化发送操作的互斥锁
    std::mutex m_sendMutex;
    // 收到完整消息包后的回调函数
    PacketHandler m_packetHandler;
};
