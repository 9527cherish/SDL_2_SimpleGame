#pragma once

#include <chrono>
#include <mutex>
#include <unordered_map>
#include <vector>
#include "messageInfo.hpp"

// ServerMonsterManager 负责服务端怪物状态、受击、掉落和重生逻辑
class ServerMonsterManager
{
public:
    struct AttackResult
    {
        bool hit = false;
        bool killed = false;
        MonsterInfo monster;
        bool spawnedDrop = false;
        DropInfo drop;
        bool spawnedProjectile = false;
        ProjectileInfo projectile;
    };

    struct ProjectileTickResult
    {
        std::vector<ProjectileInfo> projectiles;
        std::vector<MonsterInfo> monsters;
        std::vector<DropInfo> drops;
    };

    // 返回所有怪物的当前快照
    std::vector<MonsterInfo> getAllMonsters();
    // 返回所有掉落物的当前快照
    std::vector<DropInfo> getAllDrops();
    // 返回所有投射物当前快照
    std::vector<ProjectileInfo> getAllProjectiles();
    // 根据当前在线玩家推进怪物 AI 并返回变化列表
    std::vector<MonsterInfo> updateMonsters(const std::vector<PlayerInfo>& players);
    // 对指定怪物应用一次伤害并返回变化结果
    AttackResult hitMonster(const MonsterHitRequest& request, const PlayerInfo* attacker);
    // 推进所有投射物并返回变化结果
    ProjectileTickResult updateProjectiles();
    // 拾取指定掉落物并返回变化结果
    bool pickDrop(const DropPickupRequest& request, DropInfo& updatedDrop);
    // 刷新应当重生的怪物并返回变化列表
    std::vector<MonsterInfo> refreshMonsters();

private:
    friend class GameService;
    ServerMonsterManager();

    struct MonsterRuntime
    {
        MonsterInfo info;
        std::chrono::steady_clock::time_point respawnTime;
        int spawnX = 0;
        int spawnY = 0;
        int patrolDirection = 1;
        float preciseX = 0.0f;
        float preciseY = 0.0f;
        float moveSpeed = 72.0f;
        int chaseRange = 260;
        int patrolRange = 56;
        std::string dropName;
        std::string dropSpritePath;
        std::string dropItemType;
        std::string dropWeaponType;
        int dropPower = 0;
        std::chrono::steady_clock::time_point lastUpdateTime;
    };

    struct ProjectileRuntime
    {
        ProjectileInfo info;
        float originX = 0.0f;
        float originY = 0.0f;
        int maxDistance = 520;
        std::chrono::steady_clock::time_point lastUpdateTime;
    };

    void applyDamageUnlocked(MonsterRuntime& runtime, int damage, AttackResult& result);

private:
    // 保护怪物与掉落状态的互斥锁
    std::mutex m_mutex;
    // 以怪物 ID 为键的怪物状态表
    std::unordered_map<int, MonsterRuntime> m_monsters;
    // 以掉落物 ID 为键的掉落状态表
    std::unordered_map<int, DropInfo> m_drops;
    // 以投射物 ID 为键的投射物状态表
    std::unordered_map<int, ProjectileRuntime> m_projectiles;
    // 下一个分配给掉落物的 ID
    int m_nextDropId = 1;
    // 下一个投射物 ID
    int m_nextProjectileId = 1;
};
