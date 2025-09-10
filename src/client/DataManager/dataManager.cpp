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
    m_loadData = std::async(std::launch::async, &LoadXml::parseAllPersonaXml);
}

void DataManager::getData(std::vector<Persona> &personas)
{
    if(!m_bLoadData){
        getDataFromFuture();
    }
    personas = m_personas;
}

void DataManager::getDataFromFuture()
{
    if(!m_bLoadData){
        m_personas =  m_loadData.get();
        m_bLoadData = true;
    }
}
