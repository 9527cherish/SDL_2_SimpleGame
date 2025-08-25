#include "dataManager.hpp"

DataManager DataManager::getInstance()
{
    static DataManager instance;
    return instance;
}

DataManager::DataManager()
{
    m_xmlPath = "../../images/tmwa/";
    m_graphicsPath = "../../images/tmwa/graphics/sprites/";
    m_npcXmlPath = "npcs/_include.xml";
    m_neutralXmlPath = "graphics/sprites/model/neutral.xml";
}

DataManager::~DataManager()
{
}

void DataManager::initData()
{
}
