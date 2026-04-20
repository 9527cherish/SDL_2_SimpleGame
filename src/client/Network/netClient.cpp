#include "Network/netClient.hpp"

#include <uuid/uuid.h>
#include <spdlog/spdlog.h>

#include "characterAction.hpp"
#include "characterDirection.hpp"
#include "dropManager.hpp"
#include "Inventory/inventoryManager.hpp"
#include "monsterManager.hpp"
#include "playerSession.hpp"
#include "projectileManager.hpp"
#include "remotePlayerStore.hpp"
#include "persona.hpp"
#include "treeManager.hpp"

NetClient::NetClient()
    : m_serverIp("127.0.0.1")
    , m_serverPort(8080)
    , m_enteredGame(false)
{
}

NetClient::~NetClient()
{
    leaveGame();
}

NetClient& NetClient::getInstance()
{
    static NetClient instance;
    return instance;
}

bool NetClient::enterGame()
{
    if (m_localUuid.empty()) {
        m_localUuid = generateUuid();
    }
    PlayerSession::getInstance().setPlayerUuid(m_localUuid);

    if (!m_transport.connectTo(
            m_serverIp,
            m_serverPort,
            [this](int msgId, const json& data) { handleMessage(msgId, data); })) {
        return false;
    }

    RemotePlayerStore::getInstance().clear();
    TreeManager::getInstance().clearTrees();
    MonsterManager::getInstance().clearMonsters();
    DropManager::getInstance().clearDrops();
    ProjectileManager::getInstance().clear();
    m_enteredGame = true;
    // 先拉一份当前在线玩家快照，再发送自己的最新状态，后进入的客户端也能看到已有角色。
    sendMessage(ENUM_MSG_SYNC_PLAYERS_REQUEST, json{{"uuid", m_localUuid}});
    sendMessage(ENUM_MSG_SYNC_TREES_REQUEST, json{{"uuid", m_localUuid}});
    sendMessage(ENUM_MSG_SYNC_MONSTERS_REQUEST, json{{"uuid", m_localUuid}});
    sendMessage(ENUM_MSG_SYNC_DROPS_REQUEST, json{{"uuid", m_localUuid}});
    sendMessage(ENUM_MSG_SYNC_INVENTORY_REQUEST, json{{"uuid", m_localUuid}});
    sendMessage(ENUM_MSG_SYNC_PROJECTILES_REQUEST, json{{"uuid", m_localUuid}});
    syncCurrentPlayer();
    return true;
}

void NetClient::leaveGame()
{
    if (m_enteredGame && m_transport.isConnected() && !m_localUuid.empty())
    {
        PlayerInfo playerInfo;
        playerInfo.uuid = m_localUuid;
        sendMessage(ENUM_MSG_DELETE_PLAYER_REQUEST, playerInfo);
    }

    m_enteredGame = false;
    m_transport.disconnect();
    RemotePlayerStore::getInstance().clear();
    TreeManager::getInstance().clearTrees();
    MonsterManager::getInstance().clearMonsters();
    DropManager::getInstance().clearDrops();
    ProjectileManager::getInstance().clear();
    InventoryManager::getInstance().clear();
}

void NetClient::syncCurrentPlayer()
{
    if (!m_enteredGame || !m_transport.isConnected()) {
        return;
    }

    PlayerInfo playerInfo = buildCurrentPlayerInfo();
    if (playerInfo.uuid.empty() || playerInfo.personaId < 0) {
        return;
    }

    sendMessage(ENUM_MSG_REGISTER_UPDATE_PLAYER_REQUEST, playerInfo);
}

void NetClient::hitTree(int treeId, int damage)
{
    if (!m_enteredGame || !m_transport.isConnected() || treeId <= 0) {
        return;
    }

    TreeHitRequest request;
    request.playerUuid = m_localUuid;
    request.treeId = treeId;
    request.damage = damage;
    sendMessage(ENUM_MSG_HIT_TREE_REQUEST, request);
}

void NetClient::hitMonster(int monsterId, int damage, const std::string& weaponType)
{
    if (!m_enteredGame || !m_transport.isConnected() || monsterId <= 0) {
        return;
    }

    MonsterHitRequest request;
    request.playerUuid = m_localUuid;
    request.monsterId = monsterId;
    request.damage = damage;
    request.weaponType = weaponType;
    sendMessage(ENUM_MSG_HIT_MONSTER_REQUEST, request);
}

void NetClient::pickDrop(int dropId)
{
    if (!m_enteredGame || !m_transport.isConnected() || dropId <= 0) {
        return;
    }

    DropPickupRequest request;
    request.playerUuid = m_localUuid;
    request.dropId = dropId;
    sendMessage(ENUM_MSG_PICK_DROP_REQUEST, request);
}

void NetClient::syncInventory()
{
    if (!m_enteredGame || !m_transport.isConnected() || m_localUuid.empty()) {
        return;
    }

    sendMessage(ENUM_MSG_SYNC_INVENTORY_REQUEST, json{{"uuid", m_localUuid}});
}

void NetClient::syncProjectiles()
{
    if (!m_enteredGame || !m_transport.isConnected() || m_localUuid.empty()) {
        return;
    }

    sendMessage(ENUM_MSG_SYNC_PROJECTILES_REQUEST, json{{"uuid", m_localUuid}});
}

void NetClient::handleMessage(int msgId, const json& data)
{
    switch (msgId)
    {
    case ENUM_MSG_REGISTER_UPDATE_PLAYER_RESPONSE:
    {
        PlayerInfo playerInfo = data.get<PlayerInfo>();
        if (playerInfo.uuid == m_localUuid) {
            return;
        }
        RemotePlayerStore::getInstance().syncRemotePlayer(playerInfo);
        break;
    }
    case ENUM_MSG_DELETE_PLAYER_RESPONSE:
    {
        PlayerInfo playerInfo = data.get<PlayerInfo>();
        if (playerInfo.uuid == m_localUuid) {
            return;
        }
        RemotePlayerStore::getInstance().deleteRemotePlayer(playerInfo.uuid);
        break;
    }
    case ENUM_MSG_SYNC_PLAYERS_RESPONSE:
    {
        RemotePlayerStore::getInstance().clear();
        std::vector<PlayerInfo> players = data.value("players", std::vector<PlayerInfo>());
        for (const PlayerInfo& playerInfo : players)
        {
            if (playerInfo.uuid == m_localUuid) {
                continue;
            }
            RemotePlayerStore::getInstance().syncRemotePlayer(playerInfo);
        }
        break;
    }
    case ENUM_MSG_SYNC_TREES_RESPONSE:
    {
        TreeManager::getInstance().syncTrees(data.value("trees", std::vector<TreeInfo>()));
        break;
    }
    case ENUM_MSG_HIT_TREE_RESPONSE:
    case ENUM_MSG_UPDATE_TREE_PUSH:
    {
        TreeManager::getInstance().updateTree(data.get<TreeInfo>());
        break;
    }
    case ENUM_MSG_SYNC_MONSTERS_RESPONSE:
    {
        MonsterManager::getInstance().syncMonsters(data.value("monsters", std::vector<MonsterInfo>()));
        break;
    }
    case ENUM_MSG_HIT_MONSTER_RESPONSE:
    case ENUM_MSG_UPDATE_MONSTER_PUSH:
    {
        MonsterManager::getInstance().updateMonster(data.get<MonsterInfo>());
        break;
    }
    case ENUM_MSG_SYNC_DROPS_RESPONSE:
    {
        DropManager::getInstance().syncDrops(data.value("drops", std::vector<DropInfo>()));
        break;
    }
    case ENUM_MSG_PICK_DROP_RESPONSE:
    case ENUM_MSG_UPDATE_DROP_PUSH:
    {
        DropManager::getInstance().updateDrop(data.get<DropInfo>());
        break;
    }
    case ENUM_MSG_SYNC_INVENTORY_RESPONSE:
    case ENUM_MSG_UPDATE_INVENTORY_PUSH:
    {
        InventorySnapshot snapshot = data.get<InventorySnapshot>();
        if (snapshot.playerUuid == m_localUuid) {
            InventoryManager::getInstance().syncInventory(snapshot);
        }
        break;
    }
    case ENUM_MSG_SYNC_PROJECTILES_RESPONSE:
    {
        ProjectileManager::getInstance().syncProjectiles(data.value("projectiles", std::vector<ProjectileInfo>()));
        break;
    }
    case ENUM_MSG_UPDATE_PROJECTILE_PUSH:
    {
        ProjectileManager::getInstance().updateProjectile(data.get<ProjectileInfo>());
        break;
    }
    default:
        break;
    }
}

void NetClient::sendMessage(ENUM_MSG_TYPE msgType, const json& data)
{
    if (!m_transport.isConnected()) {
        return;
    }

    m_transport.sendPacket(msgType, data);
}

PlayerInfo NetClient::buildCurrentPlayerInfo()
{
    PlayerInfo playerInfo;
    std::shared_ptr<Persona> persona = PlayerSession::getInstance().currentPersona();
    if (persona == nullptr) {
        return playerInfo;
    }

    playerInfo.uuid = m_localUuid;
    playerInfo.personaId = PlayerSession::getInstance().currentPersonaIndex();
    playerInfo.name = "player_" + m_localUuid.substr(0, 8);
    // 网络层同步的是人物模板编号 + 位置 + 动作状态，远端据此重建并驱动对应 Persona。
    playerInfo.x = persona->x();
    playerInfo.y = persona->y();
    playerInfo.action = ActionMapper::to_string(persona->action());
    playerInfo.direction = DirectionMapper::to_string(persona->direction());
    playerInfo.parts = persona->partSyncInfos();
    return playerInfo;
}

std::string NetClient::generateUuid()
{
    uuid_t uuidValue;
    uuid_generate(uuidValue);
    char uuidStr[37] = {0};
    uuid_unparse(uuidValue, uuidStr);
    return uuidStr;
}
