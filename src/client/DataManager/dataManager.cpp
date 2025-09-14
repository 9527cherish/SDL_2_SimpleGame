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

void DataManager::getData(std::vector<std::shared_ptr<Persona>>& personas)

{
    if(!m_bLoadData){
        getDataFromFuture();
    }
    personas = m_personas;
}

void DataManager::setCurrentPerson(std::shared_ptr<Persona> persona)
{
    m_pCurrentPerson = persona;
}

void DataManager::setCurrentPerson(int number)
{
    m_pCurrentPerson = m_personas[number];
}

std::shared_ptr<Persona> DataManager::currentPersona()
{
    return m_pCurrentPerson;
}

void DataManager::getDataFromFuture()
{
    if(!m_bLoadData){
        m_personas =  m_loadData.get();
        m_bLoadData = true;
    }
}
