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
    PlayerInfo playerInfo;
    playerInfo.uuid = js["uuid"];
    playerInfo.name = js["name"];
    playerInfo.x = js["x"];
    playerInfo.y = js["y"];
    
    playerInfo.texture.name = js["texture"]["name"] ;
    playerInfo.texture.x = js["texture"]["x"];
    playerInfo.texture.y = js["texture"]["y"];
    playerInfo.texture.width = js["texture"]["width"];
    playerInfo.texture.height = js["texture"]["height"];
    
    return playerInfo;
}
