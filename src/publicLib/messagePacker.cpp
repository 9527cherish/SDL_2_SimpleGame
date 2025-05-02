#include "messagePacker.hpp"
#include <cstring>
#include <stdexcept>
#include <arpa/inet.h>  // for htonl/ntohl on Linux

std::vector<char> MessagePacker::pack(uint32_t msgId, const std::string& payload) {
    std::vector<char> buffer;
    uint32_t totalLength = 4 + payload.size(); // msgId + payload
    uint32_t netLength = htonl(totalLength);
    uint32_t netMsgId = htonl(msgId);

    buffer.insert(buffer.end(),
                  reinterpret_cast<char*>(&netLength),
                  reinterpret_cast<char*>(&netLength) + 4);

    buffer.insert(buffer.end(),
                  reinterpret_cast<char*>(&netMsgId),
                  reinterpret_cast<char*>(&netMsgId) + 4);

    buffer.insert(buffer.end(), payload.begin(), payload.end());
    return buffer;
}

void MessagePacker::Unpacker::append(const char* data, size_t len) {
    buffer.insert(buffer.end(), data, data + len);
    processBuffer();
}

void MessagePacker::Unpacker::onMessage(const MessageCallback& cb) {
    callback = cb;
}

bool MessagePacker::Unpacker::unpack(std::string packeMsg, uint32_t &msgId, std::string &payload)
{
    try
    {
        if (packeMsg.size() < 4) return false;

        uint32_t netLen;
        std::memcpy(&netLen, packeMsg.data(), 4);
        uint32_t totalLength = ntohl(netLen);

        if (packeMsg.size() < totalLength + 4) return false;
               uint32_t netMsgId;
        std::memcpy(&netMsgId, packeMsg.data() + 4, 4);
        msgId = ntohl(netMsgId);

        payload.assign(packeMsg.begin() + 8, packeMsg.begin() + 4 + totalLength);
        return true;
    }
    catch(const std::exception& e)
    {
        
    }
    
}

void MessagePacker::Unpacker::processBuffer() {
    while (true) {
        if (buffer.size() < 4) return;

        uint32_t netLen;
        std::memcpy(&netLen, buffer.data(), 4);
        uint32_t totalLength = ntohl(netLen);

        if (buffer.size() < totalLength + 4) return;

        uint32_t netMsgId;
        std::memcpy(&netMsgId, buffer.data() + 4, 4);
        uint32_t msgId = ntohl(netMsgId);

        std::string payload(buffer.begin() + 8, buffer.begin() + 4 + totalLength);

        if (callback) callback(msgId, payload);

        buffer.erase(buffer.begin(), buffer.begin() + 4 + totalLength);
    }
}
