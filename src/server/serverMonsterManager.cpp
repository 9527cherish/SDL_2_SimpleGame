#include "serverMonsterManager.hpp"

#include <algorithm>
#include <cmath>

namespace {
constexpr int kMonsterRespawnSeconds = 18;
constexpr float kMaxUpdateStepSeconds = 0.12f;
constexpr float kProjectileHitRadius = 18.0f;
const char* kSlimeSpritePath = "../images/tmwa/graphics/sprites/monsters/slime.xml";
const char* kBeeSpritePath = "../images/tmwa/graphics/sprites/monsters/bee.xml";
const char* kBatSpritePath = "../images/tmwa/graphics/sprites/monsters/bat.xml";
const char* kMaggotSpritePath = "../images/tmwa/graphics/sprites/monsters/maggot.xml";
const char* kScorpionSpritePath = "../images/tmwa/graphics/sprites/monsters/scorpion.xml";
const char* kPinkieSpritePath = "../images/tmwa/graphics/sprites/monsters/pinkie.xml";
const char* kDuckSpritePath = "../images/tmwa/graphics/sprites/monsters/duck.xml";

struct MonsterSpawnConfig
{
    int monsterId = 0;
    const char* name = "";
    int x = 0;
    int y = 0;
    int maxHp = 1;
    const char* spritePath = "";
    float moveSpeed = 72.0f;
    int chaseRange = 260;
    int patrolRange = 56;
    const char* dropName = "";
    const char* dropSpritePath = "";
    const char* dropItemType = "material";
    const char* dropWeaponType = "";
    int dropPower = 0;
};

const MonsterSpawnConfig kMonsterConfigs[] = {
    {1, "Green Slime", 420, 520, 4, kSlimeSpritePath, 64.0f, 240, 60, "Slime Jelly", "../images/tmwa/graphics/items/generic/slimeball.png"},
    {2, "Green Slime", 760, 540, 4, kSlimeSpritePath, 64.0f, 240, 60, "Short Sword", "../images/tmwa/graphics/items/equipment/weapon/sword-shortsword.png", "weapon", "sword", 2},
    {3, "Forest Bee", 980, 520, 3, kBeeSpritePath, 108.0f, 300, 84, "Hunter Bow", "../images/tmwa/graphics/items/equipment/weapon/bow-shortbow.png", "weapon", "bow", 2},
    {4, "Forest Bee", 1180, 560, 3, kBeeSpritePath, 108.0f, 300, 84, "Bee Stinger", "../images/tmwa/graphics/items/generic/stinger.png"},
    {5, "Cave Bat", 620, 760, 5, kBatSpritePath, 128.0f, 320, 96, "Apprentice Wand", "../images/tmwa/graphics/items/equipment/weapon/wand.png", "weapon", "wand", 3},
    {6, "Cave Bat", 840, 820, 5, kBatSpritePath, 128.0f, 320, 96, "Bat Wing", "../images/tmwa/graphics/items/equipment/amulets/angel-wings.png"},
    {7, "Wild Maggot", 320, 860, 6, kMaggotSpritePath, 58.0f, 210, 44, "Bone Darts", "../images/tmwa/graphics/items/equipment/ammo/bonedarts.png"},
    {8, "Wild Maggot", 540, 920, 6, kMaggotSpritePath, 58.0f, 210, 44, "Iron Spear", "../images/tmwa/graphics/items/equipment/weapon/polearm-spear.png", "weapon", "spear", 3},
    {9, "Sand Scorpion", 1320, 760, 8, kScorpionSpritePath, 82.0f, 280, 76, "Battle Axe", "../images/tmwa/graphics/items/equipment/weapon/axe-axe.png", "weapon", "axe", 4},
    {10, "Sand Scorpion", 1460, 860, 8, kScorpionSpritePath, 82.0f, 280, 76, "Dark Talisman", "../images/tmwa/graphics/items/equipment/charms/darktalisman.png"},
    {11, "Pinkie", 1040, 980, 10, kPinkieSpritePath, 76.0f, 260, 72, "Pinkie Helmet", "../images/tmwa/graphics/items/equipment/head/pinkie-helmet.png"},
    {12, "Duckling", 740, 1080, 7, kDuckSpritePath, 72.0f, 230, 64, "Fourleaf Amulet", "../images/tmwa/graphics/items/equipment/amulets/fourleaf-amulet.png"}
};

float distanceSquared(int x1, int y1, int x2, int y2)
{
    const float dx = static_cast<float>(x1 - x2);
    const float dy = static_cast<float>(y1 - y2);
    return dx * dx + dy * dy;
}

float distanceSquaredFloat(float x1, float y1, float x2, float y2)
{
    const float dx = x1 - x2;
    const float dy = y1 - y2;
    return dx * dx + dy * dy;
}
}

ServerMonsterManager::ServerMonsterManager()
{
    for (const MonsterSpawnConfig& config : kMonsterConfigs)
    {
        MonsterRuntime runtime;
        runtime.info.monsterId = config.monsterId;
        runtime.info.name = config.name;
        runtime.info.x = config.x;
        runtime.info.y = config.y;
        runtime.info.hp = config.maxHp;
        runtime.info.maxHp = config.maxHp;
        runtime.info.state = ENUM_MONSTER_STATE_ALIVE;
        runtime.info.respawnSeconds = kMonsterRespawnSeconds;
        runtime.info.spritePath = config.spritePath;
        runtime.respawnTime = std::chrono::steady_clock::time_point::min();
        runtime.spawnX = config.x;
        runtime.spawnY = config.y;
        runtime.preciseX = static_cast<float>(config.x);
        runtime.preciseY = static_cast<float>(config.y);
        runtime.moveSpeed = config.moveSpeed;
        runtime.chaseRange = config.chaseRange;
        runtime.patrolRange = config.patrolRange;
        runtime.dropName = config.dropName;
        runtime.dropSpritePath = config.dropSpritePath;
        runtime.dropItemType = config.dropItemType;
        runtime.dropWeaponType = config.dropWeaponType;
        runtime.dropPower = config.dropPower;
        runtime.lastUpdateTime = std::chrono::steady_clock::now();
        m_monsters[config.monsterId] = runtime;
    }
}

std::vector<MonsterInfo> ServerMonsterManager::getAllMonsters()
{
    refreshMonsters();

    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<MonsterInfo> monsters;
    monsters.reserve(m_monsters.size());
    for (const auto& [monsterId, runtime] : m_monsters)
    {
        (void)monsterId;
        monsters.emplace_back(runtime.info);
    }
    std::sort(monsters.begin(), monsters.end(), [](const MonsterInfo& lhs, const MonsterInfo& rhs) {
        return lhs.monsterId < rhs.monsterId;
    });
    return monsters;
}

std::vector<DropInfo> ServerMonsterManager::getAllDrops()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<DropInfo> drops;
    drops.reserve(m_drops.size());
    for (const auto& [dropId, drop] : m_drops)
    {
        (void)dropId;
        drops.emplace_back(drop);
    }
    std::sort(drops.begin(), drops.end(), [](const DropInfo& lhs, const DropInfo& rhs) {
        return lhs.dropId < rhs.dropId;
    });
    return drops;
}

std::vector<ProjectileInfo> ServerMonsterManager::getAllProjectiles()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<ProjectileInfo> projectiles;
    projectiles.reserve(m_projectiles.size());
    for (const auto& [projectileId, runtime] : m_projectiles)
    {
        (void)projectileId;
        if (runtime.info.state != ENUM_PROJECTILE_STATE_ACTIVE) {
            continue;
        }
        projectiles.emplace_back(runtime.info);
    }
    std::sort(projectiles.begin(), projectiles.end(), [](const ProjectileInfo& lhs, const ProjectileInfo& rhs) {
        return lhs.projectileId < rhs.projectileId;
    });
    return projectiles;
}

std::vector<MonsterInfo> ServerMonsterManager::updateMonsters(const std::vector<PlayerInfo>& players)
{
    std::vector<MonsterInfo> changedMonsters = refreshMonsters();

    std::lock_guard<std::mutex> lock(m_mutex);
    const auto now = std::chrono::steady_clock::now();

    for (auto& [monsterId, runtime] : m_monsters)
    {
        (void)monsterId;
        if (runtime.info.state != ENUM_MONSTER_STATE_ALIVE) {
            continue;
        }

        const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - runtime.lastUpdateTime).count();
        runtime.lastUpdateTime = now;
        if (elapsed <= 0) {
            continue;
        }

        const float deltaSeconds = std::min(kMaxUpdateStepSeconds, static_cast<float>(elapsed) / 1000.0f);
        const int oldX = runtime.info.x;
        const int oldY = runtime.info.y;

        const PlayerInfo* nearestPlayer = nullptr;
        float nearestDistanceSq = static_cast<float>(runtime.chaseRange * runtime.chaseRange);
        for (const PlayerInfo& player : players)
        {
            const float currentDistanceSq = distanceSquared(runtime.info.x, runtime.info.y, player.x, player.y);
            if (currentDistanceSq > nearestDistanceSq) {
                continue;
            }

            nearestDistanceSq = currentDistanceSq;
            nearestPlayer = &player;
        }

        if (nearestPlayer != nullptr)
        {
            const float dx = static_cast<float>(nearestPlayer->x) - runtime.preciseX;
            const float dy = static_cast<float>(nearestPlayer->y) - runtime.preciseY;
            const float length = std::sqrt(dx * dx + dy * dy);
            if (length > 1.0f)
            {
                const float step = runtime.moveSpeed * deltaSeconds;
                runtime.preciseX += dx / length * std::min(step, length);
                runtime.preciseY += dy / length * std::min(step, length);
            }
        }
        else
        {
            runtime.preciseX += runtime.patrolDirection * runtime.moveSpeed * 0.55f * deltaSeconds;
            if (std::abs(runtime.preciseX - static_cast<float>(runtime.spawnX)) > runtime.patrolRange)
            {
                runtime.patrolDirection *= -1;
                runtime.preciseX = std::clamp(runtime.preciseX,
                                              static_cast<float>(runtime.spawnX - runtime.patrolRange),
                                              static_cast<float>(runtime.spawnX + runtime.patrolRange));
            }

            runtime.preciseY = static_cast<float>(runtime.spawnY) +
                               std::sin(static_cast<float>(now.time_since_epoch().count() + runtime.info.monsterId * 1000) / 400000000.0f) * 8.0f;
        }

        runtime.info.x = static_cast<int>(std::round(runtime.preciseX));
        runtime.info.y = static_cast<int>(std::round(runtime.preciseY));
        if (runtime.info.x != oldX || runtime.info.y != oldY) {
            changedMonsters.emplace_back(runtime.info);
        }
    }

    return changedMonsters;
}

void ServerMonsterManager::applyDamageUnlocked(MonsterRuntime& runtime, int damage, AttackResult& result)
{
    result.hit = true;
    runtime.info.hp = std::max(0, runtime.info.hp - std::max(1, damage));
    if (runtime.info.hp == 0)
    {
        result.killed = true;
        runtime.info.state = ENUM_MONSTER_STATE_DEAD;
        runtime.respawnTime = std::chrono::steady_clock::now() + std::chrono::seconds(runtime.info.respawnSeconds);

        DropInfo drop;
        drop.dropId = m_nextDropId++;
        drop.name = runtime.dropName;
        drop.x = runtime.info.x;
        drop.y = runtime.info.y + 18;
        drop.state = ENUM_DROP_STATE_AVAILABLE;
        drop.spritePath = runtime.dropSpritePath;
        drop.sourceMonsterId = runtime.info.monsterId;
        drop.itemType = runtime.dropItemType;
        drop.weaponType = runtime.dropWeaponType;
        drop.power = runtime.dropPower;
        m_drops[drop.dropId] = drop;
        result.spawnedDrop = true;
        result.drop = drop;
    }

    result.monster = runtime.info;
}

ServerMonsterManager::AttackResult ServerMonsterManager::hitMonster(const MonsterHitRequest& request, const PlayerInfo* attacker)
{
    refreshMonsters();

    std::lock_guard<std::mutex> lock(m_mutex);
    AttackResult result;
    auto iter = m_monsters.find(request.monsterId);
    if (iter == m_monsters.end()) {
        return result;
    }

    MonsterRuntime& runtime = iter->second;
    if (runtime.info.state != ENUM_MONSTER_STATE_ALIVE) {
        result.monster = runtime.info;
        return result;
    }

    if ((request.weaponType == "bow" || request.weaponType == "wand") && attacker != nullptr)
    {
        ProjectileRuntime projectile;
        projectile.info.projectileId = m_nextProjectileId++;
        projectile.info.ownerUuid = request.playerUuid;
        projectile.info.targetMonsterId = request.monsterId;
        projectile.info.x = static_cast<float>(attacker->x);
        projectile.info.y = static_cast<float>(attacker->y - 36);
        projectile.info.damage = std::max(1, request.damage);
        projectile.info.projectileType = request.weaponType == "bow" ? "arrow" : "magic";
        projectile.info.state = ENUM_PROJECTILE_STATE_ACTIVE;
        projectile.originX = projectile.info.x;
        projectile.originY = projectile.info.y;
        projectile.maxDistance = request.weaponType == "bow" ? 620 : 520;
        projectile.lastUpdateTime = std::chrono::steady_clock::now();

        const float dx = static_cast<float>(runtime.info.x) - projectile.info.x;
        const float dy = static_cast<float>(runtime.info.y - 28) - projectile.info.y;
        const float length = std::sqrt(std::max(1.0f, dx * dx + dy * dy));
        const float projectileSpeed = request.weaponType == "bow" ? 560.0f : 420.0f;
        projectile.info.vx = dx / length * projectileSpeed;
        projectile.info.vy = dy / length * projectileSpeed;
        m_projectiles[projectile.info.projectileId] = projectile;
        result.spawnedProjectile = true;
        result.projectile = projectile.info;
        return result;
    }

    applyDamageUnlocked(runtime, request.damage, result);
    return result;
}

ServerMonsterManager::ProjectileTickResult ServerMonsterManager::updateProjectiles()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    ProjectileTickResult tickResult;
    const auto now = std::chrono::steady_clock::now();

    std::vector<int> toRemove;
    for (auto& [projectileId, projectile] : m_projectiles)
    {
        if (projectile.info.state != ENUM_PROJECTILE_STATE_ACTIVE) {
            toRemove.emplace_back(projectileId);
            continue;
        }

        const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - projectile.lastUpdateTime).count();
        projectile.lastUpdateTime = now;
        if (elapsed <= 0) {
            continue;
        }

        const float deltaSeconds = std::min(kMaxUpdateStepSeconds, static_cast<float>(elapsed) / 1000.0f);
        projectile.info.x += projectile.info.vx * deltaSeconds;
        projectile.info.y += projectile.info.vy * deltaSeconds;

        tickResult.projectiles.emplace_back(projectile.info);

        const float travelDistanceSq = distanceSquaredFloat(projectile.originX, projectile.originY,
                                                            projectile.info.x, projectile.info.y);
        if (travelDistanceSq > static_cast<float>(projectile.maxDistance * projectile.maxDistance))
        {
            projectile.info.state = ENUM_PROJECTILE_STATE_EXPIRED;
            tickResult.projectiles.emplace_back(projectile.info);
            toRemove.emplace_back(projectileId);
            continue;
        }

        auto monsterIter = m_monsters.find(projectile.info.targetMonsterId);
        if (monsterIter == m_monsters.end() || monsterIter->second.info.state != ENUM_MONSTER_STATE_ALIVE) {
            projectile.info.state = ENUM_PROJECTILE_STATE_EXPIRED;
            tickResult.projectiles.emplace_back(projectile.info);
            toRemove.emplace_back(projectileId);
            continue;
        }

        MonsterRuntime& runtime = monsterIter->second;
        const float hitDistanceSq = distanceSquaredFloat(projectile.info.x, projectile.info.y,
                                                         static_cast<float>(runtime.info.x),
                                                         static_cast<float>(runtime.info.y - 28));
        if (hitDistanceSq > kProjectileHitRadius * kProjectileHitRadius) {
            continue;
        }

        projectile.info.state = ENUM_PROJECTILE_STATE_IMPACT;
        tickResult.projectiles.emplace_back(projectile.info);
        toRemove.emplace_back(projectileId);

        AttackResult result;
        applyDamageUnlocked(runtime, projectile.info.damage, result);
        tickResult.monsters.emplace_back(result.monster);
        if (result.spawnedDrop) {
            tickResult.drops.emplace_back(result.drop);
        }
    }

    for (int projectileId : toRemove)
    {
        m_projectiles.erase(projectileId);
    }

    return tickResult;
}

bool ServerMonsterManager::pickDrop(const DropPickupRequest& request, DropInfo& updatedDrop)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto iter = m_drops.find(request.dropId);
    if (iter == m_drops.end()) {
        return false;
    }

    if (iter->second.state != ENUM_DROP_STATE_AVAILABLE) {
        updatedDrop = iter->second;
        return false;
    }

    iter->second.state = ENUM_DROP_STATE_PICKED;
    updatedDrop = iter->second;
    return true;
}

std::vector<MonsterInfo> ServerMonsterManager::refreshMonsters()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<MonsterInfo> changedMonsters;
    const auto now = std::chrono::steady_clock::now();

    for (auto& [monsterId, runtime] : m_monsters)
    {
        (void)monsterId;
        if (runtime.info.state == ENUM_MONSTER_STATE_ALIVE) {
            continue;
        }

        if (now < runtime.respawnTime) {
            continue;
        }

        runtime.info.hp = runtime.info.maxHp;
        runtime.info.state = ENUM_MONSTER_STATE_ALIVE;
        runtime.info.x = runtime.spawnX;
        runtime.info.y = runtime.spawnY;
        runtime.preciseX = static_cast<float>(runtime.spawnX);
        runtime.preciseY = static_cast<float>(runtime.spawnY);
        runtime.respawnTime = std::chrono::steady_clock::time_point::min();
        runtime.lastUpdateTime = now;
        changedMonsters.emplace_back(runtime.info);
    }

    return changedMonsters;
}
