#pragma once 
#include "partBase.hpp"



class Persona
{
public:
    void setId(uint id);
    void renderer(SDL_Renderer* renderer, int x, int y);
    void rendererCurPersona(SDL_Renderer* renderer, int x, int y);
    void update(const CharaAction& actionName, const CharaDirection& direction, int deltaTime);
    void addPartBase(const PartBase& part);
    void printPersonaInfo();
    bool handleEvent(const SDL_Event& e, Uint32& lastFrameTime, Uint32& deltaTime, bool move = true);

private:
    uint m_id;                         // NPC ID
    std::vector<PartBase> m_spriteParts;  // 精灵部件列表

    int m_x = 0;                     // 角色的X坐标
    int m_y = 0;                     // 角色的Y坐标
    CharaAction m_actionName;
    CharaDirection m_direction;
};