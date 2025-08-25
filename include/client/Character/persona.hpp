#pragma once 
#include "partBase.hpp"



class Persona
{
public:
    void setId(uint id);


private:
    uint m_id;                         // NPC ID
    std::vector<PartBase> m_spriteParts;  // 精灵部件列表
};