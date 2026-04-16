#pragma once

#include <string>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

// PartSyncInfo 用于同步角色某个部件当前帧的显示状态
struct PartSyncInfo
{
    // 当前帧在动作序列中的下标
    int frameIndex = 0;
    // 当前帧在图集中的实际帧值
    int frameValue = 0;
    // 当前帧在 X 方向上的偏移
    int offsetX = 0;
    // 当前帧在 Y 方向上的偏移
    int offsetY = 0;
    // 当前帧的播放延迟
    int delay = 0;
};

// 把部件同步信息序列化为 JSON。
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

// 从 JSON 中反序列化部件同步信息。
inline void from_json(const json& js, PartSyncInfo& part)
{
    part.frameIndex = js.value("frameIndex", 0);
    part.frameValue = js.value("frameValue", 0);
    part.offsetX = js.value("offsetX", 0);
    part.offsetY = js.value("offsetY", 0);
    part.delay = js.value("delay", 0);
}

// PlayerInfo 表示客户端和服务端之间同步的玩家快照
struct PlayerInfo
{
    // 玩家唯一标识
    std::string uuid;
    // 玩家显示名称
    std::string name;
    // 使用的人物模板编号
    int personaId = -1;
    // 玩家世界坐标 X
    int x = 0;
    // 玩家世界坐标 Y
    int y = 0;
    // 玩家当前动作
    std::string action = "stand";
    // 玩家当前方向
    std::string direction = "down";
    // 玩家各部件的逐帧同步信息
    std::vector<PartSyncInfo> parts;
};

// 把玩家快照序列化为 JSON。
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

// 从 JSON 中反序列化玩家快照。
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
