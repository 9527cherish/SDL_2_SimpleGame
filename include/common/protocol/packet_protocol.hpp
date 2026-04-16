#pragma once

#include <string>
#include "json.hpp"
#include "common/protocol/msg_types.hpp"

using json = nlohmann::json;

// 把消息号和数据对象打包成按行分隔的 JSON 字符串。
inline std::string packMessage(const ENUM_MSG_TYPE msgType, const json& data)
{
    json packet;
    packet["msgId"] = static_cast<int>(msgType);
    packet["data"] = data;
    return packet.dump() + "\n";
}

// PacketEnvelope 表示解析后的单条网络数据包
struct PacketEnvelope
{
    // 当前数据包对应的消息号
    int msgId = -1;
    // 当前数据包携带的业务数据
    json data = json::object();
};

// 把一行 JSON 文本解析成统一的数据包结构。
inline bool parsePacketLine(const std::string& line, PacketEnvelope& envelope)
{
    json packet = json::parse(line, nullptr, false);
    if (packet.is_discarded()) {
        return false;
    }

    envelope.msgId = packet.value("msgId", -1);
    envelope.data = packet.value("data", json::object());
    return true;
}
