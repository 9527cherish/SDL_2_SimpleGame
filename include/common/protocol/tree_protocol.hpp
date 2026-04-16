#pragma once

#include <string>
#include "json.hpp"
#include "common/protocol/msg_types.hpp"

using json = nlohmann::json;

// TreeInfo 表示一棵树在联机中的同步状态
struct TreeInfo
{
    // 树木唯一 ID
    int treeId = 0;
    // 树木世界坐标 X
    int x = 0;
    // 树木世界坐标 Y
    int y = 0;
    // 树木当前生命值
    int hp = 0;
    // 树木最大生命值
    int maxHp = 0;
    // 树木当前状态
    int state = ENUM_TREE_STATE_ALIVE;
    // 树桩到重生所需秒数
    int respawnSeconds = 0;
    // 树木图片路径
    std::string spritePath;
};

// 把树木状态序列化为 JSON。
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

// 从 JSON 中反序列化树木状态。
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

// TreeHitRequest 表示客户端发起的一次砍树请求
struct TreeHitRequest
{
    // 发起攻击的玩家 UUID
    std::string playerUuid;
    // 被攻击的树木 ID
    int treeId = 0;
    // 本次攻击造成的伤害
    int damage = 1;
};

// 把砍树请求序列化为 JSON。
inline void to_json(json& js, const TreeHitRequest& request)
{
    js = json::object();
    js["playerUuid"] = request.playerUuid;
    js["treeId"] = request.treeId;
    js["damage"] = request.damage;
}

// 从 JSON 中反序列化砍树请求。
inline void from_json(const json& js, TreeHitRequest& request)
{
    request.playerUuid = js.value("playerUuid", "");
    request.treeId = js.value("treeId", 0);
    request.damage = js.value("damage", 1);
}
