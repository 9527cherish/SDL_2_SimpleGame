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

    ENUM_MSG_SYNC_TREES_REQUEST,       // 同步树木列表
    ENUM_MSG_SYNC_TREES_RESPONSE,      // 树木列表回复

    ENUM_MSG_HIT_TREE_REQUEST,         // 砍树请求
    ENUM_MSG_HIT_TREE_RESPONSE,        // 砍树结果

    ENUM_MSG_UPDATE_TREE_PUSH,         // 树木状态推送

    ENUM_MSG_SENDMESSAGE_REQUEST,  // 发送消息请求
    ENUM_MSG_SENDMESSAGE_RESPONSE,    // 发送消息回复
};

enum ENUM_TREE_STATE
{
    ENUM_TREE_STATE_ALIVE = 0,
    ENUM_TREE_STATE_STUMP = 1
};

struct PartSyncInfo
{
    int frameIndex = 0;
    int frameValue = 0;
    int offsetX = 0;
    int offsetY = 0;
    int delay = 0;
};

inline void to_json(json& js, const PartSyncInfo& part)
{
    js = json{
        {"frameIndex", part.frameIndex},
        {"frameValue", part.frameValue},
        {"offsetX", part.offsetX},
        {"offsetY", part.offsetY},
        {"delay", part.delay}
    };
}

inline void from_json(const json& js, PartSyncInfo& part)
{
    part.frameIndex = js.value("frameIndex", 0);
    part.frameValue = js.value("frameValue", 0);
    part.offsetX = js.value("offsetX", 0);
    part.offsetY = js.value("offsetY", 0);
    part.delay = js.value("delay", 0);
}

struct PlayerInfo
{
    std::string uuid;         // 唯一标识
    std::string name;         // 角色名称  
    int personaId;            // 当前选择的人物模板编号
    int x;                    // 位置 x
    int y;                    // 位置 y
    std::string action;       // 当前动作
    std::string direction;    // 当前方向
    std::vector<PartSyncInfo> parts;  // 每个部件当前动画帧

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

struct TreeInfo
{
    int treeId = 0;
    int x = 0;
    int y = 0;
    int hp = 0;
    int maxHp = 0;
    int state = ENUM_TREE_STATE_ALIVE;
    int respawnSeconds = 0;
    std::string spritePath;
};

inline void to_json(json& js, const TreeInfo& tree)
{
    js = json::object();
    js["treeId"] = tree.treeId;
    js["x"] = tree.x;
    js["y"] = tree.y;
    js["hp"] = tree.hp;
    js["maxHp"] = tree.maxHp;
    js["state"] = tree.state;
    js["respawnSeconds"] = tree.respawnSeconds;
    js["spritePath"] = tree.spritePath;
}

inline void from_json(const json& js, TreeInfo& tree)
{
    tree.treeId = js.value("treeId", 0);
    tree.x = js.value("x", 0);
    tree.y = js.value("y", 0);
    tree.hp = js.value("hp", 0);
    tree.maxHp = js.value("maxHp", 0);
    tree.state = js.value("state", ENUM_TREE_STATE_ALIVE);
    tree.respawnSeconds = js.value("respawnSeconds", 0);
    tree.spritePath = js.value("spritePath", "");
}

struct TreeHitRequest
{
    std::string playerUuid;
    int treeId = 0;
    int damage = 1;
};

inline void to_json(json& js, const TreeHitRequest& request)
{
    js = json::object();
    js["playerUuid"] = request.playerUuid;
    js["treeId"] = request.treeId;
    js["damage"] = request.damage;
}

inline void from_json(const json& js, TreeHitRequest& request)
{
    request.playerUuid = js.value("playerUuid", "");
    request.treeId = js.value("treeId", 0);
    request.damage = js.value("damage", 1);
}

inline void to_json(json& js, const PlayerInfo& player)
{
    js = json::object();
    js["uuid"] = player.uuid;
    js["name"] = player.name;
    js["personaId"] = player.personaId;
    js["x"] = player.x;
    js["y"] = player.y;
    js["action"] = player.action;
    js["direction"] = player.direction;
    js["parts"] = player.parts;
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
    if (js.contains("parts") && js["parts"].is_array()) {
        player.parts = js["parts"].get<std::vector<PartSyncInfo>>();
    } else {
        player.parts.clear();
    }
}

inline std::string packMessage(const ENUM_MSG_TYPE msgType, const json& data)
{
    json packet;
    packet["msgId"] = static_cast<int>(msgType);
    packet["data"] = data;
    return packet.dump() + "\n";
}
