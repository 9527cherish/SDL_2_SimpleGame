#pragma once

#include "partBase.hpp"
#include "sceneStruct.hpp"
#include <string>

class Persona;

// RemotePersona 表示通过网络同步得到的远端角色显示实例
class RemotePersona
{
public:
    RemotePersona() = default;
    RemotePersona(const RemotePersona& remotePersona);
    RemotePersona(const Persona& persona);

    // 设置远端角色 ID。
    void setId(uint id);
    // 给远端角色添加一个部件。
    void addPartBase(const PartBase& part);
    // 直接应用远端同步过来的动作、方向和位置。
    void setState(const CharaAction& actionName, const CharaDirection& direction, int x, int y);
    // 应用远端角色各部件的逐帧同步信息。
    void applyPartSyncInfos(const std::vector<PartSyncInfo>& partSyncInfos);
    // 以脚点为锚点渲染远端角色。
    void rendererCurPersonaFootScaled(SDL_Renderer* renderer, int footX, int footY, float scale);

    // 返回远端角色世界坐标 X。
    int x() const;
    // 返回远端角色世界坐标 Y。
    int y() const;
    // 返回远端角色当前动作。
    CharaAction action() const;
    // 返回远端角色当前方向。
    CharaDirection direction() const;

private:
    // 远端角色 ID
    uint m_id = 0;
    // 远端角色的所有可渲染部件
    std::vector<PartBase> m_spriteParts;
    // 远端角色世界坐标 X
    int m_x = SCREEN_WIDTH / 2;
    // 远端角色世界坐标 Y
    int m_y = SCREEN_HEIGHT / 2;
    // 远端角色当前动作
    CharaAction m_actionName = CharaAction::STAND;
    // 远端角色当前方向
    CharaDirection m_direction = CharaDirection::DOWN;
};
