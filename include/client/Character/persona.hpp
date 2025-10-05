#pragma once 
#include "partBase.hpp"



class Persona
{
public:
    void setId(uint id);
    void renderer(SDL_Renderer* renderer, int x, int y);
    void update(const CharaAction& actionName, const CharaDirection& direction, int deltaTime);
    void addPartBase(const PartBase& part);
    void printPersonaInfo();

private:
    uint m_id;                         // NPC ID
    std::vector<PartBase> m_spriteParts;  // 精灵部件列表
};