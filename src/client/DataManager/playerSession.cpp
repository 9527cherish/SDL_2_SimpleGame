#include "playerSession.hpp"
#include "assetRepository.hpp"

PlayerSession& PlayerSession::getInstance()
{
    static PlayerSession instance;
    return instance;
}

void PlayerSession::setCurrentPersona(std::shared_ptr<Persona> persona)
{
    m_currentPersona = persona;
    m_currentPersonaIndex = -1;
}

void PlayerSession::selectPersona(int index)
{
    std::shared_ptr<Persona> persona = AssetRepository::getInstance().createPersona(index);
    if (persona == nullptr) {
        return;
    }

    m_currentPersona = persona;
    m_currentPersonaIndex = index;
    spdlog::info("设置当前人物索引: {}", index);
}

std::shared_ptr<Persona> PlayerSession::currentPersona()
{
    return m_currentPersona;
}

int PlayerSession::currentPersonaIndex() const
{
    return m_currentPersonaIndex;
}

void PlayerSession::setPlayerUuid(const std::string& uuid)
{
    m_playerUuid = uuid;
}

const std::string& PlayerSession::playerUuid() const
{
    return m_playerUuid;
}
