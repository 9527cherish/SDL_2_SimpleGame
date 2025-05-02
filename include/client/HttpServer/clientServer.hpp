#pragma once 


#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "messagePacker.hpp"

// 线程安全队列模板
template<typename T>
class SafeQueue {
    std::queue<T> queue;
    mutable std::mutex mtx;
public:
    void push(const T& item) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(item);
    }

    bool pop(T& item) {
        std::lock_guard<std::mutex> lock(mtx);
        if(queue.empty()) return false;
        item = queue.front();
        queue.pop();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.empty();
    }
};


class ClientServer{

public:

    ClientServer();
    ~ClientServer(); 

    void io_worker();
    void reconnect();
    
    void send_message(const uint32_t msgid, const std::string& msg);
    bool get_message(std::string& msg);

    void dealServerData();

    void dealPackCallBack(uint32_t msgId, const std::string& payload);

    bool pop(std::tuple<uint32_t, std::string>& msg);

private:
    int sock;
    std::atomic<bool> running;
    std::thread io_thread;
    
    std::string serverIP;
    uint32_t serverPort;

    SafeQueue<std::string> send_queue;
    SafeQueue<std::string> recv_queue;

    std::mutex m_csqueue;
    std::queue<std::tuple<uint32_t, std::string>> recv_callback_queue;

    MessagePacker::Unpacker unpacker;
};

