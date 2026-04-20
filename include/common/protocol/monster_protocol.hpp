#pragma once

#include <string>
#include "json.hpp"
#include "common/protocol/msg_types.hpp"

using json = nlohmann::json;

// MonsterInfo 表示一只怪物在联机中的同步状态
struct MonsterInfo
{
    // 怪物唯一 ID
    int monsterId = 0;
    // 怪物显示名称
    std::string name;
    // 怪物世界坐标 X
    int x = 0;
    // 怪物世界坐标 Y
    int y = 0;
    // 怪物当前生命值
    int hp = 0;
    // 怪物最大生命值
    int maxHp = 0;
    // 怪物当前状态
    int state = ENUM_MONSTER_STATE_ALIVE;
    // 怪物重生时间
    int respawnSeconds = 0;
    // 怪物图片路径
    std::string spritePath;
};

// 把怪物状态序列化为 JSON
inline void to_json(json& js, const MonsterInfo& monster)
{
    js = json::object();
    js["monsterId"] = monster.monsterId;
    js["name"] = monster.name;
    js["x"] = monster.x;
    js["y"] = monster.y;
    js["hp"] = monster.hp;
    js["maxHp"] = monster.maxHp;
    js["state"] = monster.state;
    js["respawnSeconds"] = monster.respawnSeconds;
    js["spritePath"] = monster.spritePath;
}

// 从 JSON 中反序列化怪物状态
inline void from_json(const json& js, MonsterInfo& monster)
{
    monster.monsterId = js.value("monsterId", 0);
    monster.name = js.value("name", "");
    monster.x = js.value("x", 0);
    monster.y = js.value("y", 0);
    monster.hp = js.value("hp", 0);
    monster.maxHp = js.value("maxHp", 0);
    monster.state = js.value("state", ENUM_MONSTER_STATE_ALIVE);
    monster.respawnSeconds = js.value("respawnSeconds", 0);
    monster.spritePath = js.value("spritePath", "");
}

// MonsterHitRequest 表示客户端发起的一次攻击怪物请求
struct MonsterHitRequest
{
    // 发起攻击的玩家 UUID
    std::string playerUuid;
    // 被攻击的怪物 ID
    int monsterId = 0;
    // 本次攻击造成的伤害
    int damage = 1;
    // 当前使用的武器类型
    std::string weaponType;
};

// 把攻击怪物请求序列化为 JSON
inline void to_json(json& js, const MonsterHitRequest& request)
{
    js = json::object();
    js["playerUuid"] = request.playerUuid;
    js["monsterId"] = request.monsterId;
    js["damage"] = request.damage;
    js["weaponType"] = request.weaponType;
}

// 从 JSON 中反序列化攻击怪物请求
inline void from_json(const json& js, MonsterHitRequest& request)
{
    request.playerUuid = js.value("playerUuid", "");
    request.monsterId = js.value("monsterId", 0);
    request.damage = js.value("damage", 1);
    request.weaponType = js.value("weaponType", "");
}
