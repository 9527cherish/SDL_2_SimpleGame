#include "persona.hpp"
#include "characterStruct.hpp"
#include "loadXml.hpp"

void Persona::setId(uint id)
{
    m_id = id;
}

void Persona::renderer(SDL_Renderer *renderer, int x, int y)
{
    CharaAction currentAction = CharaAction::STAND;
    CharaDirection currentDir = CharaDirection::DOWN;

    update(currentAction, currentDir, 60);

    for(PartBase& partSprite : m_spriteParts)
    {
        partSprite.initTexture(renderer);
        partSprite.render(renderer, currentAction, currentDir, x, y);
    }
}

void Persona::update(const CharaAction &actionName, const CharaDirection &direction, int deltaTime)
{
    for(PartBase& partSprite : m_spriteParts)
    {
        partSprite.update(actionName, direction, deltaTime);
    }
}

void Persona::addPartBase(const PartBase &part)
{
    m_spriteParts.emplace_back(part);
}

void Persona::printPersonaInfo()
{
    for(PartBase& partSprite : m_spriteParts)
    {
        LoadXml::printImageXml(partSprite.imageSet());
        LoadXml::printSpriteData(partSprite.spriteData());
    }
}
