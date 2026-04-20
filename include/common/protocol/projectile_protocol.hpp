#pragma once

#include <string>
#include <vector>
#include "json.hpp"
#include "common/protocol/msg_types.hpp"

using json = nlohmann::json;

struct ProjectileInfo
{
    int projectileId = 0;
    std::string ownerUuid;
    int targetMonsterId = 0;
    float x = 0.0f;
    float y = 0.0f;
    float vx = 0.0f;
    float vy = 0.0f;
    int damage = 0;
    std::string projectileType;
    int state = ENUM_PROJECTILE_STATE_ACTIVE;
};

inline void to_json(json& js, const ProjectileInfo& projectile)
{
    js = json::object();
    js["projectileId"] = projectile.projectileId;
    js["ownerUuid"] = projectile.ownerUuid;
    js["targetMonsterId"] = projectile.targetMonsterId;
    js["x"] = projectile.x;
    js["y"] = projectile.y;
    js["vx"] = projectile.vx;
    js["vy"] = projectile.vy;
    js["damage"] = projectile.damage;
    js["projectileType"] = projectile.projectileType;
    js["state"] = projectile.state;
}

inline void from_json(const json& js, ProjectileInfo& projectile)
{
    projectile.projectileId = js.value("projectileId", 0);
    projectile.ownerUuid = js.value("ownerUuid", "");
    projectile.targetMonsterId = js.value("targetMonsterId", 0);
    projectile.x = js.value("x", 0.0f);
    projectile.y = js.value("y", 0.0f);
    projectile.vx = js.value("vx", 0.0f);
    projectile.vy = js.value("vy", 0.0f);
    projectile.damage = js.value("damage", 0);
    projectile.projectileType = js.value("projectileType", "");
    projectile.state = js.value("state", ENUM_PROJECTILE_STATE_ACTIVE);
}
