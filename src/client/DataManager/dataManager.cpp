#include "dataManager.hpp"
#include "loadXml.hpp"
#include "characterAction.hpp"
#include "characterDirection.hpp"


DataManager& DataManager::getInstance()
{
    static DataManager instance;
    return instance;
}

DataManager::DataManager()
    : m_bLoadData(false)
{

}

DataManager::~DataManager()
{
}

void DataManager::initData()
{
    spdlog::info("开始异步加载人物数据");
    m_loadData = std::async(std::launch::async, &LoadXml::parseAllPersonaXml);
}

void DataManager::getData(std::vector<std::shared_ptr<Persona>>& personas)
{
    if(!m_bLoadData){
        getDataFromFuture();
    }
    // spdlog::debug("读取人物数据，总数: {}", m_personas.size());
    personas = m_personas;
}

void DataManager::setCurrentPerson(std::shared_ptr<Persona> persona)
{
    // 在这儿应该生成一份拷贝，拷贝CurrentPerson， 否则会影响从前的m_personas数据
    m_pCurrentPerson = persona;
    m_iCurrentPersonIndex = -1;
}

void DataManager::setCurrentPerson(int number)
{
    // 此处应该生成一份拷贝，拷贝CurrentPerson， 否则会影响从前的m_personas数据
    if (number < 0 || number >= static_cast<int>(m_personas.size())) {
        spdlog::error("选择人物失败，索引越界: {}", number);
        return;
    }
    spdlog::info("设置当前人物索引: {}", number);
    m_pCurrentPerson =  std::make_shared<Persona>(*(m_personas[number]));
    m_iCurrentPersonIndex = number;
}

std::shared_ptr<Persona> DataManager::currentPersona()
{
    return m_pCurrentPerson;
}

int DataManager::currentPersonaIndex() const
{
    return m_iCurrentPersonIndex;
}

std::shared_ptr<Persona> DataManager::createPersona(int number)
{
    if(!m_bLoadData){
        getDataFromFuture();
    }

    if (number < 0 || number >= static_cast<int>(m_personas.size())) {
        spdlog::error("创建人物失败，索引越界: {}", number);
        return nullptr;
    }

    return std::make_shared<Persona>(*(m_personas[number]));
}

void DataManager::syncRemotePersona(const PlayerInfo& playerInfo)
{
    std::lock_guard<std::mutex> lock(m_remotePersonasMutex);
    auto iter = m_remotePersonas.find(playerInfo.uuid);

    if (iter == m_remotePersonas.end() || iter->second.personaId != playerInfo.personaId || iter->second.persona == nullptr)
    {
        std::shared_ptr<Persona> persona = createPersona(playerInfo.personaId);
        if (persona == nullptr) {
            return;
        }

        RemotePersonaData remoteData;
        remoteData.personaId = playerInfo.personaId;
        remoteData.persona = persona;
        iter = m_remotePersonas.emplace(playerInfo.uuid, remoteData).first;
    }

    iter->second.persona->setState(ActionMapper::from_string(playerInfo.action),
                                   DirectionMapper::from_string(playerInfo.direction),
                                   playerInfo.x,
                                   playerInfo.y);
}

void DataManager::deleteRemotePersona(const std::string& uuid)
{
    std::lock_guard<std::mutex> lock(m_remotePersonasMutex);
    auto iter = m_remotePersonas.find(uuid);
    if (iter != m_remotePersonas.end()) {
        m_remotePersonas.erase(iter);
    }
}

void DataManager::clearRemotePersonas()
{
    std::lock_guard<std::mutex> lock(m_remotePersonasMutex);
    m_remotePersonas.clear();
}

void DataManager::getRemotePersonas(std::vector<std::shared_ptr<Persona>>& personas)
{
    std::lock_guard<std::mutex> lock(m_remotePersonasMutex);
    personas.clear();
    for (const auto& [uuid, remoteData] : m_remotePersonas)
    {
        (void)uuid;
        if (remoteData.persona == nullptr) {
            continue;
        }
        personas.emplace_back(std::make_shared<Persona>(*(remoteData.persona)));
    }
}

void DataManager::getDataFromFuture()
{
    if(!m_bLoadData){
        spdlog::info("等待人物数据加载完成");
        m_personas =  m_loadData.get();
        m_bLoadData = true;
        spdlog::info("人物数据加载完成，数量: {}", m_personas.size());
    }
}
