#include "characterManager.hpp"


CharacterManager::CharacterManager()
{
}

CharacterManager::~CharacterManager()
{
}

CharacterManager &CharacterManager::getInstanse()
{
    static CharacterManager instance;
    return instance;
}

void CharacterManager::addPlayer(PlayerInfo player)
{
    std::lock_guard<std::mutex> lock(mutexPlayerInfo);
    m_mapPlayerInfo[player.uuid] = player;

}

void CharacterManager::deletePlayer(PlayerInfo palyer)
{
    std::lock_guard<std::mutex> lock(mutexPlayerInfo);
    auto iter = m_mapPlayerInfo.find(palyer.uuid);
    if(iter != m_mapPlayerInfo.end())
    {
        m_mapPlayerInfo.erase(iter);
    }
}

PlayerInfo CharacterManager::generatePlayer(json js)
{
    return js.get<PlayerInfo>();
}

std::vector<PlayerInfo> CharacterManager::getAllPlayers()
{
    std::lock_guard<std::mutex> lock(mutexPlayerInfo);
    std::vector<PlayerInfo> players;
    players.reserve(m_mapPlayerInfo.size());
    for (const auto& [uuid, player] : m_mapPlayerInfo)
    {
        (void)uuid;
        players.emplace_back(player);
    }
    return players;
}
