#include "persona.hpp"
#include "characterStruct.hpp"

void Persona::setId(uint id)
{
    m_id = id;
}

void Persona::renderSpritePart(SDL_Renderer *renderer, int &x, int &y)
{
    CharaAction currentAction = CharaAction::STAND;
    CharaDirection currentDir = CharaDirection::DOWN;

    for(PartBase& partSprite : m_spriteParts)
    {
        partSprite.initTexture(renderer);
        partSprite.render(renderer, currentAction, currentDir, x, y);
    }
}

void Persona::addPartBase(const PartBase &part)
{
    m_spriteParts.emplace_back(part);
}
