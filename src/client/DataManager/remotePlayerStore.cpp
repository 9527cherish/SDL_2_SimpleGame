#include "remotePlayerStore.hpp"
#include "assetRepository.hpp"
#include "characterAction.hpp"
#include "characterDirection.hpp"

RemotePlayerStore& RemotePlayerStore::getInstance()
{
    static RemotePlayerStore instance;
    return instance;
}

void RemotePlayerStore::syncRemotePlayer(const PlayerInfo& playerInfo)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto iter = m_remotePersonas.find(playerInfo.uuid);
    const CharaAction nextAction = ActionMapper::from_string(playerInfo.action);
    const CharaDirection nextDirection = DirectionMapper::from_string(playerInfo.direction);

    if (iter == m_remotePersonas.end() || iter->second.personaId != playerInfo.personaId || iter->second.persona == nullptr)
    {
        std::shared_ptr<Persona> persona = AssetRepository::getInstance().createPersona(playerInfo.personaId);
        if (persona == nullptr) {
            return;
        }

        RemotePersonaData remoteData;
        remoteData.personaId = playerInfo.personaId;
        remoteData.persona = std::make_shared<RemotePersona>(*persona);
        remoteData.targetX = playerInfo.x;
        remoteData.targetY = playerInfo.y;
        remoteData.targetAction = nextAction;
        remoteData.targetDirection = nextDirection;
        iter = m_remotePersonas.emplace(playerInfo.uuid, remoteData).first;
    }

    iter->second.targetX = playerInfo.x;
    iter->second.targetY = playerInfo.y;
    iter->second.targetAction = nextAction;
    iter->second.targetDirection = nextDirection;
    iter->second.persona->setState(nextAction, nextDirection, playerInfo.x, playerInfo.y);
    iter->second.persona->applyPartSyncInfos(playerInfo.parts);
}

void RemotePlayerStore::deleteRemotePlayer(const std::string& uuid)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto iter = m_remotePersonas.find(uuid);
    if (iter != m_remotePersonas.end()) {
        m_remotePersonas.erase(iter);
    }
}

void RemotePlayerStore::clear()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_remotePersonas.clear();
}

void RemotePlayerStore::advance(Uint32 deltaTime)
{
    (void)deltaTime;
}

void RemotePlayerStore::getPlayers(std::vector<std::shared_ptr<RemotePersona>>& players)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    players.clear();
    for (const auto& [uuid, remoteData] : m_remotePersonas)
    {
        (void)uuid;
        if (remoteData.persona == nullptr) {
            continue;
        }
        players.emplace_back(std::make_shared<RemotePersona>(*(remoteData.persona)));
    }
}
