#pragma once

#include "partBase.hpp"
#include "sceneStruct.hpp"
#include <string>

class Persona;

class RemotePersona
{
public:
    RemotePersona() = default;
    RemotePersona(const RemotePersona& remotePersona);
    RemotePersona(const Persona& persona);

    void setId(uint id);
    void addPartBase(const PartBase& part);
    void setState(const CharaAction& actionName, const CharaDirection& direction, int x, int y);
    void applyPartSyncInfos(const std::vector<PartSyncInfo>& partSyncInfos);
    void rendererCurPersonaFootScaled(SDL_Renderer* renderer, int footX, int footY, float scale);

    int x() const;
    int y() const;
    CharaAction action() const;
    CharaDirection direction() const;

private:
    uint m_id = 0;
    std::vector<PartBase> m_spriteParts;
    int m_x = SCREEN_WIDTH / 2;
    int m_y = SCREEN_HEIGHT / 2;
    CharaAction m_actionName = CharaAction::STAND;
    CharaDirection m_direction = CharaDirection::DOWN;
};
