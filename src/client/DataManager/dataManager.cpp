#include "dataManager.hpp"
#include "loadXml.hpp"

DataManager& DataManager::getInstance()
{
    static DataManager instance;
    return instance;
}

DataManager::DataManager()
{

}

DataManager::~DataManager()
{
}

void DataManager::initData()
{
    m_personas = LoadXml::parseAllPersonaXml();
}

bool DataManager::getPersonas(std::vector<Persona> &persons)
{
    if(m_personas.empty())
        return false;

    persons = m_personas;
    return true;
}
