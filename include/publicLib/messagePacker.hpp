#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <functional>

#if defined(_WIN32) || defined(_WIN64)
    #define MSG_API __declspec(dllexport)
#else
    #define MSG_API __attribute__((visibility("default")))
#endif

class MSG_API MessagePacker {
public:
    static std::vector<char> pack(uint32_t msgId, const std::string& payload);

    class Unpacker {
    public:
        using MessageCallback = std::function<void(uint32_t msgId, const std::string& payload)>;

        void append(const char* data, size_t len);
        void onMessage(const MessageCallback& cb);

        bool unpack(std::string packeMsg, uint32_t& msgId, std::string& payload);

    private:
        void processBuffer();

        std::vector<char> buffer;
        MessageCallback callback;
    };
};
