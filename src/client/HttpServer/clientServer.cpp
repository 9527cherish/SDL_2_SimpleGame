#include "./HttpServer/clientServer.hpp"
// #include "clientServer.h"
#include "player.hpp"
#include "chatBox.hpp"
#include <spdlog/spdlog.h>
#include "clientComonFunc.hpp"
#include "clientServer.hpp"


ClientServer::ClientServer()
{
    sock = -1;
    running = true;

    serverIP = "127.0.0.1";
    serverPort = 8080;

    reconnect();

    io_thread = std::thread(&ClientServer::io_worker, this);

    // 指定unpacker的回调函数
    unpacker.onMessage([&](uint32_t msgId, const std::string& payload)
    {
        this->dealPackCallBack(msgId, payload);
    });

}

ClientServer::~ClientServer()
{
    running = false;
    if(io_thread.joinable()) io_thread.join();
    close(sock);
}

void ClientServer::io_worker()
{
    
    while(running) {
        // 发送处理
        std::string send_msg;
        while(send_queue.pop(send_msg)) {
            if(send_msg.empty())
            {
                continue;
            }
            spdlog::info("send_msg" +send_msg);
            spdlog::info("send_msg.size" + std::to_string(send_msg.size()));
            send(sock, send_msg.c_str(), send_msg.size(), 0);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void ClientServer::reconnect()
{
    // close(sock);
    // while(running) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        fcntl(sock, F_SETFL, O_NONBLOCK);
        sockaddr_in serv_addr{};
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(serverPort);
        inet_pton(AF_INET, serverIP.c_str(), &serv_addr.sin_addr);

        if(connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == 0) {
            // break;
        }
        // std::this_thread::sleep_for(std::chrono::seconds(1));
    // }
}

void ClientServer::send_message(const uint32_t msgid, const std::string &msg)
{
    // 减少数据的发送量，人物不移动不发送
    if(!g_player->isMove() && msgid == ENUM_MSG_REGISTER_UPDATE_PLAYER_REQUEST) 
        return;

    if(msg.empty() && msgid == ENUM_MSG_SENDMESSAGE_REQUEST)
        return;
    // 将要发送的数据清空
    else if (!msg.empty() && msgid == ENUM_MSG_SENDMESSAGE_REQUEST)
        g_chatBox->setSendText("");

    auto data = MessagePacker::pack(msgid, msg);
    std::string sendMsg(data.data(), data.size());
    // spdlog::info("发送通知：" + sendMsg);
    send_queue.push(sendMsg);
}

bool ClientServer::get_message(std::string &msg)
{
    return recv_queue.pop(msg);
}

void ClientServer::dealServerData()
{
    while(1)
    {
        // 接收处理
        char buffer[4096];
        ssize_t bytes = recv(sock, buffer, sizeof(buffer), 0);
        if(bytes > 0) {
            // unpacker.append(buffer, bytes);
            // recv_queue.push(std::string(buffer, bytes));
            
        } else{
            continue;
        }
        // std::tuple<uint32_t, std::string> msgTuple;
        // bool isPop = pop(msgTuple);
        std::string recvMsg = std::string(buffer, bytes);
        // bool isPop = recv_queue.pop(recvMsg);
        // if(isPop)
        // {
            // uint32_t msgid = std::get<0>(msgTuple);
            // std::string msg = std::get<1>(msgTuple);
            // spdlog::info("收到从服务器传来的msg:" +  msg);
            uint32_t msgId;
            std::string payload;
            unpacker.unpack(recvMsg, msgId, payload);
            json msgJson = json::parse(payload);
            switch (msgId)
            {
                case ENUM_MSG_REGISTER_UPDATE_PLAYER_RESPONSE:
                {
                    // 开始处理数据
                    // 判断收到的更新数据是否是当前玩家，如果是，则不处理,如果不是，在则放到敌人里
                    
                    if(msgJson["uuid"] == g_player->getUuid())
                    {
                        continue;
                    }
                    else  
                    {
                        PlayerInfo enemy = g_player->generatePlayerInfo(msgJson);
                        g_player->addAndUpgradeEnemy(enemy);

                    }
                    break;
                }
                case ENUM_MSG_DELETE_PLAYER_RESPONSE:
                {
                    PlayerInfo enemy = g_player->generatePlayerInfo(msgJson);
                    g_player->deleteEnemy(enemy);
                    break;
                }
                case ENUM_MSG_SENDMESSAGE_RESPONSE:
                {
                    // 将数据塞入chatBox中
                    if(msgJson["uuid"] == g_player->getUuid())
                    {
                        continue;
                    }
                    else{
                        g_chatBox->addMessage(msgJson["sendMsg"]);
                    }
                    
                    break;
                }    
            default:
                break;
            }


            
        // }
        // else{
        //     std::this_thread::sleep_for(std::chrono::seconds(10));
        // }
    }
}

void ClientServer::dealPackCallBack(uint32_t msgId, const std::string &payload)
{
    // spdlog::info("收到消息,消息id为:" + std::to_string(msgId));
    std::lock_guard<std::mutex> lock(m_csqueue);
    recv_callback_queue.push(std::make_tuple(msgId, payload));
}

bool ClientServer::pop(std::tuple<uint32_t, std::string> &msg)
{
    std::lock_guard<std::mutex> lock(m_csqueue);
    if(recv_callback_queue.empty()) 
        return false;
    msg = recv_callback_queue.front();
    recv_callback_queue.pop();
    return true;
}
