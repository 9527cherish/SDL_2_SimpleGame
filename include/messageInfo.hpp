#pragma once

#include <string>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

enum ENUM_MSG_TYPE
{
    ENUM_MSG_REGISTER_UPDATE_PLAYER_REQUEST = 0,  // 人物注册及更新
    ENUM_MSG_REGISTER_UPDATE_PLAYER_RESPONSE = 1,    // 人物广播回复

    ENUM_MSG_DELETE_PLAYER_REQUEST,    // 删除人物请求（某个客户端异常下线的场景）
    ENUM_MSG_DELETE_PLAYER_RESPONSE,   // 删除人物回复

    ENUM_MSG_SYNC_PLAYERS_REQUEST,     // 同步当前在线人物列表
    ENUM_MSG_SYNC_PLAYERS_RESPONSE,    // 当前在线人物列表回复

    ENUM_MSG_SENDMESSAGE_REQUEST,  // 发送消息请求
    ENUM_MSG_SENDMESSAGE_RESPONSE,    // 发送消息回复
};

struct PlayerInfo
{
    std::string uuid;         // 唯一标识
    std::string name;         // 角色名称  
    int personaId;            // 当前选择的人物模板编号
    int x;                    // 位置 x
    int y;                    // 位置 y
    std::string action;       // 当前动作
    std::string direction;    // 当前方向

    PlayerInfo()
    {
        uuid = name = "";
        action = "stand";
        direction = "down";
        personaId = -1;
        x = 0;
        y = 0;
    }
};

inline void to_json(json& js, const PlayerInfo& player)
{
    js = json{
        {"uuid", player.uuid},
        {"name", player.name},
        {"personaId", player.personaId},
        {"x", player.x},
        {"y", player.y},
        {"action", player.action},
        {"direction", player.direction}
    };
}

inline void from_json(const json& js, PlayerInfo& player)
{
    player.uuid = js.value("uuid", "");
    player.name = js.value("name", "");
    player.personaId = js.value("personaId", -1);
    player.x = js.value("x", 0);
    player.y = js.value("y", 0);
    player.action = js.value("action", "stand");
    player.direction = js.value("direction", "down");
}

inline std::string packMessage(const ENUM_MSG_TYPE msgType, const json& data)
{
    json packet;
    packet["msgId"] = static_cast<int>(msgType);
    packet["data"] = data;
    return packet.dump() + "\n";
}
