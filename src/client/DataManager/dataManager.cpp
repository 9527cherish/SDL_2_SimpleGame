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
