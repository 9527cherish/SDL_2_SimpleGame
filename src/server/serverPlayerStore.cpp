#include "serverPlayerStore.hpp"

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
