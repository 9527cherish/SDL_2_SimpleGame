#include "serverPlayerStore.hpp"

#include <algorithm>

void ServerPlayerStore::upsert(const PlayerInfo& player)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_players[player.uuid] = player;
}

void ServerPlayerStore::removeByUuid(const std::string& uuid)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto iter = m_players.find(uuid);
    if (iter != m_players.end()) {
        m_players.erase(iter);
    }
}

PlayerInfo ServerPlayerStore::parsePlayer(const json& js) const
{
    return js.get<PlayerInfo>();
}

std::vector<PlayerInfo> ServerPlayerStore::allPlayers() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<PlayerInfo> players;
    players.reserve(m_players.size());
    for (const auto& [uuid, player] : m_players)
    {
        (void)uuid;
        players.emplace_back(player);
    }
    return players;
}

bool ServerPlayerStore::getByUuid(const std::string& uuid, PlayerInfo& player) const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto iter = m_players.find(uuid);
    if (iter == m_players.end()) {
        return false;
    }
    player = iter->second;
    return true;
}

InventorySnapshot ServerPlayerStore::addInventoryItem(const std::string& uuid, const InventoryItem& item)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    InventorySnapshot snapshot;
    if (uuid.empty()) {
        return snapshot;
    }

    std::vector<InventoryItem>& inventory = m_inventories[uuid];
    auto iter = std::find_if(inventory.begin(), inventory.end(), [&](const InventoryItem& current) {
        return current.name == item.name && current.spritePath == item.spritePath;
    });

    if (iter != inventory.end()) {
        iter->count += std::max(1, item.count);
    } else {
        InventoryItem nextItem = item;
        nextItem.count = std::max(1, nextItem.count);
        inventory.emplace_back(nextItem);
    }

    snapshot.playerUuid = uuid;
    snapshot.items = inventory;
    return snapshot;
}

InventorySnapshot ServerPlayerStore::inventoryOf(const std::string& uuid) const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    InventorySnapshot snapshot;
    snapshot.playerUuid = uuid;
    auto iter = m_inventories.find(uuid);
    if (iter != m_inventories.end()) {
        snapshot.items = iter->second;
    }
    return snapshot;
}

void ServerPlayerStore::clearInventory(const std::string& uuid)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_inventories.erase(uuid);
}
