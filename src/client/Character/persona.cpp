#include "persona.hpp"
#include "characterStruct.hpp"
#include "loadXml.hpp"

Persona::Persona(const Persona &persona)
{
    m_id = persona.m_id;
    m_spriteParts = persona.m_spriteParts; // std::vector has its own copy constructor
    m_x = persona.m_x;
    m_y = persona.m_y;
    m_actionName = persona.m_actionName;
    m_direction = persona.m_direction;
}

void Persona::setId(uint id)
{
    m_id = id;
}

void Persona::renderer(SDL_Renderer *renderer, int x, int y)
{
    CharaAction currentAction = CharaAction::STAND;
    CharaDirection currentDir = CharaDirection::DOWN; 

    for(PartBase& partSprite : m_spriteParts)
    {
        partSprite.initTexture(renderer);
        partSprite.render(renderer, currentAction, currentDir, x, y);
    }
}

void Persona::rendererCurPersona(SDL_Renderer *renderer, int x, int y)
{
    for(PartBase& partSprite : m_spriteParts)
    {
        partSprite.initTexture(renderer);
        partSprite.render(renderer, m_actionName, m_direction, x, y);
    }
}

void Persona::rendererCurPersona(SDL_Renderer *renderer)
{
    for(PartBase& partSprite : m_spriteParts)
    {
        partSprite.initTexture(renderer);
        partSprite.render(renderer, m_actionName, m_direction, m_x, m_y);
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


bool Persona::handleEvent(const SDL_Event& e, Uint32& lastFrameTime, Uint32& deltaTime, bool move)
{
    if(e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_w:
                m_direction = CharaDirection::UP;
                m_actionName = CharaAction::WALK;
                if(move)
                    m_y -= 2;

                break;
            case SDLK_s:
                m_direction = CharaDirection::DOWN;
                m_actionName = CharaAction::WALK;
                if(move)
                    m_y += 2;

                break;
            case SDLK_a:
                m_direction = CharaDirection::LEFT;
                m_actionName = CharaAction::WALK;
                if(move)
                    m_x -= 2;

                break;
            case SDLK_d:
                m_direction = CharaDirection::RIGHT;
                m_actionName = CharaAction::WALK;
                if(move)
                    m_x += 2;

                break;
            case SDLK_SPACE:
                m_actionName = CharaAction::ATTACK;
                break;
            case SDLK_1:
                m_actionName = CharaAction::STAND;
                break;
            case SDLK_2:
                m_actionName = CharaAction::SIT;
                break;
            case SDLK_3:
                m_actionName = CharaAction::DEAD;
                break;
            case SDLK_4:
                m_actionName = CharaAction::ATTACK_SWORD_STAB;
                break;
            case SDLK_5:
                m_actionName = CharaAction::ATTACK_BOW;
                break;
            case SDLK_6:
                m_actionName = CharaAction::CAST;
                break;
            default:
                break;
        }
    } 
    else if (e.type == SDL_KEYUP) {
        // 当移动键释放时，回到站立状态
        if ((e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_s || 
                e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_d) && 
            m_actionName == CharaAction::WALK) {
                m_actionName = CharaAction::STAND;
        }
    }
    Uint32 currentTime = SDL_GetTicks();
    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
    update(m_actionName, m_direction, deltaTime);
    return true;
}
