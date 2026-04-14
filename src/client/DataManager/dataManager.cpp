#include "dataManager.hpp"
#include "loadXml.hpp"


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
}

std::shared_ptr<Persona> DataManager::currentPersona()
{
    return m_pCurrentPerson;
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
