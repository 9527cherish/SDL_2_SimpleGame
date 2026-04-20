#pragma once

#include <string>
#include "json.hpp"
#include "common/protocol/msg_types.hpp"

using json = nlohmann::json;

// DropInfo 表示一件地面掉落物的同步状态
struct DropInfo
{
    // 掉落物唯一 ID
    int dropId = 0;
    // 掉落物名称
    std::string name;
    // 掉落物世界坐标 X
    int x = 0;
    // 掉落物世界坐标 Y
    int y = 0;
    // 掉落物当前状态
    int state = ENUM_DROP_STATE_AVAILABLE;
    // 掉落物图片路径
    std::string spritePath;
    // 所属怪物 ID
    int sourceMonsterId = 0;
    // 掉落物类别
    std::string itemType = "material";
    // 武器类别
    std::string weaponType;
    // 强度
    int power = 0;
};

// 把掉落物状态序列化为 JSON
inline void to_json(json& js, const DropInfo& drop)
{
    js = json::object();
    js["dropId"] = drop.dropId;
    js["name"] = drop.name;
    js["x"] = drop.x;
    js["y"] = drop.y;
    js["state"] = drop.state;
    js["spritePath"] = drop.spritePath;
    js["sourceMonsterId"] = drop.sourceMonsterId;
    js["itemType"] = drop.itemType;
    js["weaponType"] = drop.weaponType;
    js["power"] = drop.power;
}

// 从 JSON 中反序列化掉落物状态
inline void from_json(const json& js, DropInfo& drop)
{
    drop.dropId = js.value("dropId", 0);
    drop.name = js.value("name", "");
    drop.x = js.value("x", 0);
    drop.y = js.value("y", 0);
    drop.state = js.value("state", ENUM_DROP_STATE_AVAILABLE);
    drop.spritePath = js.value("spritePath", "");
    drop.sourceMonsterId = js.value("sourceMonsterId", 0);
    drop.itemType = js.value("itemType", "material");
    drop.weaponType = js.value("weaponType", "");
    drop.power = js.value("power", 0);
}

// DropPickupRequest 表示客户端发起的一次拾取请求
struct DropPickupRequest
{
    // 发起拾取的玩家 UUID
    std::string playerUuid;
    // 被拾取的掉落物 ID
    int dropId = 0;
};

// 把拾取请求序列化为 JSON
inline void to_json(json& js, const DropPickupRequest& request)
{
    js = json::object();
    js["playerUuid"] = request.playerUuid;
    js["dropId"] = request.dropId;
}

// 从 JSON 中反序列化拾取请求
inline void from_json(const json& js, DropPickupRequest& request)
{
    request.playerUuid = js.value("playerUuid", "");
    request.dropId = js.value("dropId", 0);
}
