#include "persona.hpp"


void Persona::setId(uint id)
{
    m_id = id;
}

void Persona::addPartBase(const PartBase &part)
{
    m_spriteParts.emplace_back(part);
}
