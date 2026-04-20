#include "gameService.hpp"
#include <spdlog/spdlog.h>

GameService::GameService()
{
    registerHandlers();
}

MsgHander GameService::getMsgHander(int msgId) const
{
    return m_router.resolve(msgId);
}

void GameService::clientConnection(const TcpConnectionPtr& conn)
{
    if (!conn->connected())
    {
        const std::string playerUuid = m_connections.removeConnection(conn);
        if (!playerUuid.empty())
        {
            m_players.removeByUuid(playerUuid);
            m_players.clearInventory(playerUuid);

            PlayerInfo playerInfo;
            playerInfo.uuid = playerUuid;
            m_connections.broadcast(ENUM_MSG_DELETE_PLAYER_RESPONSE, playerInfo);
        }

        conn->shutdown();
        spdlog::info("{} 断开连接！", conn->peerAddress().toIpPort());
        return;
    }

    m_connections.addConnection(conn);
    spdlog::info("接收到来自 {} 的请求", conn->peerAddress().toIpPort());
}

void GameService::reset()
{
}

void GameService::registerHandlers()
{
    m_router.registerHandler(ENUM_MSG_REGISTER_UPDATE_PLAYER_REQUEST,
                             std::bind(&GameService::dealRegisterUpdatePlayer, this, _1, _2, _3));
    m_router.registerHandler(ENUM_MSG_DELETE_PLAYER_REQUEST,
                             std::bind(&GameService::dealDeletePlayer, this, _1, _2, _3));
    m_router.registerHandler(ENUM_MSG_SYNC_PLAYERS_REQUEST,
                             std::bind(&GameService::dealSyncPlayers, this, _1, _2, _3));
    m_router.registerHandler(ENUM_MSG_SYNC_TREES_REQUEST,
                             std::bind(&GameService::dealSyncTrees, this, _1, _2, _3));
    m_router.registerHandler(ENUM_MSG_HIT_TREE_REQUEST,
                             std::bind(&GameService::dealHitTree, this, _1, _2, _3));
    m_router.registerHandler(ENUM_MSG_SYNC_MONSTERS_REQUEST,
                             std::bind(&GameService::dealSyncMonsters, this, _1, _2, _3));
    m_router.registerHandler(ENUM_MSG_HIT_MONSTER_REQUEST,
                             std::bind(&GameService::dealHitMonster, this, _1, _2, _3));
    m_router.registerHandler(ENUM_MSG_SYNC_DROPS_REQUEST,
                             std::bind(&GameService::dealSyncDrops, this, _1, _2, _3));
    m_router.registerHandler(ENUM_MSG_PICK_DROP_REQUEST,
                             std::bind(&GameService::dealPickDrop, this, _1, _2, _3));
    m_router.registerHandler(ENUM_MSG_SYNC_INVENTORY_REQUEST,
                             std::bind(&GameService::dealSyncInventory, this, _1, _2, _3));
    m_router.registerHandler(ENUM_MSG_SYNC_PROJECTILES_REQUEST,
                             std::bind(&GameService::dealSyncProjectiles, this, _1, _2, _3));
    m_router.registerHandler(ENUM_MSG_SENDMESSAGE_REQUEST,
                             std::bind(&GameService::dealSendMessage, this, _1, _2, _3));
}

void GameService::dealRegisterUpdatePlayer(const TcpConnectionPtr& conn, json js, Timestamp time)
{
    (void)time;

    broadcastTrees(m_trees.refreshTrees());

    const PlayerInfo player = m_players.parsePlayer(js);
    m_players.upsert(player);
    m_connections.bindPlayer(conn, player.uuid);

    const ServerMonsterManager::ProjectileTickResult projectileTick = m_monsters.updateProjectiles();
    broadcastMonsters(projectileTick.monsters);
    broadcastDrops(projectileTick.drops);
    broadcastProjectiles(projectileTick.projectiles);
    broadcastMonsters(m_monsters.updateMonsters(m_players.allPlayers()));
    spdlog::info("更新人物: {}", player.uuid);
    m_connections.broadcast(ENUM_MSG_REGISTER_UPDATE_PLAYER_RESPONSE, player);
}

void GameService::dealDeletePlayer(const TcpConnectionPtr& conn, json js, Timestamp time)
{
    (void)time;

    const PlayerInfo player = m_players.parsePlayer(js);
    m_players.removeByUuid(player.uuid);
    m_players.clearInventory(player.uuid);
    m_connections.unbindPlayer(conn);

    spdlog::info("删除人物: {}", player.uuid);
    m_connections.broadcast(ENUM_MSG_DELETE_PLAYER_RESPONSE, player);
}

void GameService::dealSyncPlayers(const TcpConnectionPtr& conn, json js, Timestamp time)
{
    (void)js;
    (void)time;

    json response;
    response["players"] = m_players.allPlayers();
    m_connections.sendTo(conn, ENUM_MSG_SYNC_PLAYERS_RESPONSE, response);
}

void GameService::dealSyncTrees(const TcpConnectionPtr& conn, json js, Timestamp time)
{
    (void)js;
    (void)time;

    broadcastTrees(m_trees.refreshTrees());

    json response;
    response["trees"] = m_trees.getAllTrees();
    m_connections.sendTo(conn, ENUM_MSG_SYNC_TREES_RESPONSE, response);
}

void GameService::dealHitTree(const TcpConnectionPtr& conn, json js, Timestamp time)
{
    (void)conn;
    (void)time;

    broadcastTrees(m_trees.refreshTrees());

    const TreeHitRequest request = js.get<TreeHitRequest>();
    TreeInfo treeInfo;
    if (!m_trees.hitTree(request, treeInfo)) {
        return;
    }

    m_connections.broadcast(ENUM_MSG_HIT_TREE_RESPONSE, treeInfo);
}

void GameService::dealSyncMonsters(const TcpConnectionPtr& conn, json js, Timestamp time)
{
    (void)js;
    (void)time;

    const ServerMonsterManager::ProjectileTickResult projectileTick = m_monsters.updateProjectiles();
    broadcastMonsters(projectileTick.monsters);
    broadcastDrops(projectileTick.drops);
    broadcastProjectiles(projectileTick.projectiles);
    broadcastMonsters(m_monsters.updateMonsters(m_players.allPlayers()));

    json response;
    response["monsters"] = m_monsters.getAllMonsters();
    m_connections.sendTo(conn, ENUM_MSG_SYNC_MONSTERS_RESPONSE, response);
}

void GameService::dealHitMonster(const TcpConnectionPtr& conn, json js, Timestamp time)
{
    (void)conn;
    (void)time;

    const ServerMonsterManager::ProjectileTickResult projectileTick = m_monsters.updateProjectiles();
    broadcastMonsters(projectileTick.monsters);
    broadcastDrops(projectileTick.drops);
    broadcastProjectiles(projectileTick.projectiles);
    broadcastMonsters(m_monsters.updateMonsters(m_players.allPlayers()));

    const MonsterHitRequest request = js.get<MonsterHitRequest>();
    PlayerInfo attacker;
    const PlayerInfo* attackerPtr = m_players.getByUuid(request.playerUuid, attacker) ? &attacker : nullptr;
    const ServerMonsterManager::AttackResult result = m_monsters.hitMonster(request, attackerPtr);
    if (!result.hit && !result.spawnedProjectile) {
        return;
    }

    if (result.hit) {
        m_connections.broadcast(ENUM_MSG_HIT_MONSTER_RESPONSE, result.monster);
    }
    if (result.spawnedDrop) {
        m_connections.broadcast(ENUM_MSG_UPDATE_DROP_PUSH, result.drop);
    }
    if (result.spawnedProjectile) {
        m_connections.broadcast(ENUM_MSG_UPDATE_PROJECTILE_PUSH, result.projectile);
    }
}

void GameService::dealSyncDrops(const TcpConnectionPtr& conn, json js, Timestamp time)
{
    (void)js;
    (void)time;

    json response;
    response["drops"] = m_monsters.getAllDrops();
    m_connections.sendTo(conn, ENUM_MSG_SYNC_DROPS_RESPONSE, response);
}

void GameService::dealPickDrop(const TcpConnectionPtr& conn, json js, Timestamp time)
{
    (void)time;

    const DropPickupRequest request = js.get<DropPickupRequest>();
    DropInfo updatedDrop;
    if (!m_monsters.pickDrop(request, updatedDrop)) {
        return;
    }

    m_connections.broadcast(ENUM_MSG_PICK_DROP_RESPONSE, updatedDrop);
    InventoryItem item;
    item.itemId = updatedDrop.dropId;
    item.name = updatedDrop.name;
    item.count = 1;
    item.spritePath = updatedDrop.spritePath;
    item.itemType = updatedDrop.itemType;
    item.weaponType = updatedDrop.weaponType;
    item.power = updatedDrop.power;
    const InventorySnapshot snapshot = m_players.addInventoryItem(request.playerUuid, item);
    if (!snapshot.playerUuid.empty()) {
        m_connections.sendTo(conn, ENUM_MSG_UPDATE_INVENTORY_PUSH, snapshot);
    }
}

void GameService::dealSyncInventory(const TcpConnectionPtr& conn, json js, Timestamp time)
{
    (void)time;
    sendInventory(conn, js.value("uuid", ""));
}

void GameService::dealSyncProjectiles(const TcpConnectionPtr& conn, json js, Timestamp time)
{
    (void)js;
    (void)time;

    const ServerMonsterManager::ProjectileTickResult projectileTick = m_monsters.updateProjectiles();
    broadcastMonsters(projectileTick.monsters);
    broadcastDrops(projectileTick.drops);
    broadcastProjectiles(projectileTick.projectiles);

    json response;
    response["projectiles"] = m_monsters.getAllProjectiles();
    m_connections.sendTo(conn, ENUM_MSG_SYNC_PROJECTILES_RESPONSE, response);
}

void GameService::broadcastTrees(const std::vector<TreeInfo>& trees)
{
    for (const TreeInfo& tree : trees)
    {
        m_connections.broadcast(ENUM_MSG_UPDATE_TREE_PUSH, tree);
    }
}

void GameService::broadcastMonsters(const std::vector<MonsterInfo>& monsters)
{
    for (const MonsterInfo& monster : monsters)
    {
        m_connections.broadcast(ENUM_MSG_UPDATE_MONSTER_PUSH, monster);
    }
}

void GameService::broadcastDrops(const std::vector<DropInfo>& drops)
{
    for (const DropInfo& drop : drops)
    {
        m_connections.broadcast(ENUM_MSG_UPDATE_DROP_PUSH, drop);
    }
}

void GameService::broadcastProjectiles(const std::vector<ProjectileInfo>& projectiles)
{
    for (const ProjectileInfo& projectile : projectiles)
    {
        m_connections.broadcast(ENUM_MSG_UPDATE_PROJECTILE_PUSH, projectile);
    }
}

void GameService::sendInventory(const TcpConnectionPtr& conn, const std::string& playerUuid)
{
    if (playerUuid.empty()) {
        return;
    }

    m_connections.sendTo(conn, ENUM_MSG_SYNC_INVENTORY_RESPONSE, m_players.inventoryOf(playerUuid));
}

void GameService::dealSendMessage(const TcpConnectionPtr& conn, json js, Timestamp time)
{
    (void)conn;
    (void)js;
    (void)time;
}
