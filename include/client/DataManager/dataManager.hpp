#pragma once 


#include "characterStruct.hpp"
#include "messageInfo.hpp"
#include "persona.hpp"
#include <future>
#include <map>
#include <mutex>
#include <memory>

// 管理所有数据

class DataManager
{

public:
    static DataManager& getInstance(); 

    DataManager();
    ~DataManager();

    // 初始化数据
    void initData();
    void getData(std::vector<std::shared_ptr<Persona>>& personas);

    
    void setCurrentPerson(std::shared_ptr<Persona> persona);
    void setCurrentPerson(int number);
    std::shared_ptr<Persona> currentPersona();
    int currentPersonaIndex() const;
    std::shared_ptr<Persona> createPersona(int number);

    void syncRemotePersona(const PlayerInfo& playerInfo);
    void deleteRemotePersona(const std::string& uuid);
    void clearRemotePersonas();
    void advanceRemotePersonas(Uint32 deltaTime);
    void getRemotePersonas(std::vector<std::shared_ptr<Persona>>& personas);

    // bool getPersonas(std::vector<Persona>& persons);
    // 单例将拷贝构造和赋值构造删除
    DataManager(const DataManager&) = delete;

    DataManager(const DataManager&&) = delete;
    DataManager& operator=(const DataManager&) = delete;
    DataManager& operator=(const DataManager&&) = delete;

private:
    void getDataFromFuture();
    std::vector<std::shared_ptr<Persona>> m_personas;
    bool  m_bLoadData;
    std::future<std::vector<std::shared_ptr<Persona>>> m_loadData;

    // 当前选中的人物形象
    std::shared_ptr<Persona> m_pCurrentPerson;
    int m_iCurrentPersonIndex = -1;

    struct RemotePersonaData
    {
        int personaId = -1;
        std::shared_ptr<Persona> persona;
        int targetX = 0;
        int targetY = 0;
        CharaAction targetAction = CharaAction::STAND;
        CharaDirection targetDirection = CharaDirection::DOWN;
    };

    std::map<std::string, RemotePersonaData> m_remotePersonas;
    mutable std::mutex m_remotePersonasMutex;

};
