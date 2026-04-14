#pragma once 

#include <unordered_map>
#include "messageInfo.hpp"

#include <mutex>

#include "json.hpp"

using json = nlohmann::json;

class CharacterManager
{
public:
    CharacterManager();
    ~CharacterManager();

    static CharacterManager& getInstanse();

    // 添加、删除、生成人物信息，
    void addPlayer(PlayerInfo player);
    void deletePlayer(PlayerInfo palyer);
    PlayerInfo generatePlayer(json js);

private:

    std::mutex mutexPlayerInfo;
    // key:PlayerInfo中的uuid, value :PlayerInfo
    std::unordered_map<std::string, PlayerInfo> m_mapPlayerInfo;

    


};