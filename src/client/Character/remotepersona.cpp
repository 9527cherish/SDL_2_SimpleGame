#include "remotepersona.hpp"
#include "persona.hpp"
#include <algorithm>

RemotePersona::RemotePersona(const RemotePersona& remotePersona)
{
    m_id = remotePersona.m_id;
    m_spriteParts = remotePersona.m_spriteParts;
    m_x = remotePersona.m_x;
    m_y = remotePersona.m_y;
    m_actionName = remotePersona.m_actionName;
    m_direction = remotePersona.m_direction;
}

RemotePersona::RemotePersona(const Persona& persona)
{
    m_id = persona.id();
    m_spriteParts = persona.spriteParts();
    m_x = persona.x();
    m_y = persona.y();
    m_actionName = persona.action();
    m_direction = persona.direction();
}

void RemotePersona::setId(uint id)
{
    m_id = id;
}

void RemotePersona::addPartBase(const PartBase& part)
{
    m_spriteParts.emplace_back(part);
}

void RemotePersona::setState(const CharaAction& actionName, const CharaDirection& direction, int x, int y)
{
    m_actionName = actionName;
    m_direction = direction;
    m_x = x;
    m_y = y;
}

void RemotePersona::applyPartSyncInfos(const std::vector<PartSyncInfo>& partSyncInfos)
{
    const size_t partCount = std::min(m_spriteParts.size(), partSyncInfos.size());
    for (size_t i = 0; i < partCount; ++i)
    {
        m_spriteParts[i].applySyncInfo(partSyncInfos[i]);
    }
}

void RemotePersona::rendererCurPersonaFootScaled(SDL_Renderer* renderer, int footX, int footY, float scale)
{
    int drawX = static_cast<int>(footX - 32.0f * scale);
    int drawY = static_cast<int>(footY - 64.0f * scale);

    for(PartBase& partSprite : m_spriteParts)
    {
        partSprite.initTexture(renderer);
        partSprite.renderScaled(renderer, m_actionName, m_direction, drawX, drawY, scale);
    }
}

int RemotePersona::x() const
{
    return m_x;
}

int RemotePersona::y() const
{
    return m_y;
}

CharaAction RemotePersona::action() const
{
    return m_actionName;
}

CharaDirection RemotePersona::direction() const
{
    return m_direction;
}
