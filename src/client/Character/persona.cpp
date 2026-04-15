#include "persona.hpp"
#include "characterStruct.hpp"
#include "loadXml.hpp"
#include <algorithm>

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

    SDL_Rect bounds = previewBounds(currentAction, currentDir);
    int drawX = x - bounds.x - bounds.w / 2;
    int drawY = y - bounds.y - bounds.h / 2;

    for(PartBase& partSprite : m_spriteParts)
    {
        partSprite.initTexture(renderer);
        partSprite.render(renderer, currentAction, currentDir, drawX, drawY);
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

void Persona::rendererCurPersonaScaled(SDL_Renderer *renderer, int x, int y, float scale)
{
    SDL_Rect bounds = previewBounds(m_actionName, m_direction);
    int drawX = static_cast<int>(x - (bounds.x + bounds.w / 2.0f) * scale);
    int drawY = static_cast<int>(y - (bounds.y + bounds.h / 2.0f) * scale);

    for(PartBase& partSprite : m_spriteParts)
    {
        partSprite.initTexture(renderer);
        partSprite.renderScaled(renderer, m_actionName, m_direction, drawX, drawY, scale);
    }
}

void Persona::rendererCurPersonaScaled(SDL_Renderer *renderer, float scale)
{
    rendererCurPersonaScaled(renderer, m_x, m_y, scale);
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
    bool handled = false;
    if(e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_w:
            case SDLK_UP:
            case SDLK_KP_8:
                m_direction = CharaDirection::UP;
                m_actionName = CharaAction::WALK;
                if(move)
                    m_y -= 2;
                handled = true;

                break;
            case SDLK_s:
            case SDLK_DOWN:
            case SDLK_KP_2:
                m_direction = CharaDirection::DOWN;
                m_actionName = CharaAction::WALK;
                if(move)
                    m_y += 2;
                handled = true;

                break;
            case SDLK_a:
            case SDLK_LEFT:
            case SDLK_KP_4:
                m_direction = CharaDirection::LEFT;
                m_actionName = CharaAction::WALK;
                if(move)
                    m_x -= 2;
                handled = true;

                break;
            case SDLK_d:
            case SDLK_RIGHT:
            case SDLK_KP_6:
                m_direction = CharaDirection::RIGHT;
                m_actionName = CharaAction::WALK;
                if(move)
                    m_x += 2;
                handled = true;

                break;
            case SDLK_SPACE:
                m_actionName = CharaAction::ATTACK;
                handled = true;
                break;
            case SDLK_1:
                m_actionName = CharaAction::STAND;
                handled = true;
                break;
            case SDLK_2:
                m_actionName = CharaAction::SIT;
                handled = true;
                break;
            case SDLK_3:
                m_actionName = CharaAction::DEAD;
                handled = true;
                break;
            case SDLK_4:
                m_actionName = CharaAction::ATTACK_SWORD_STAB;
                handled = true;
                break;
            case SDLK_5:
                m_actionName = CharaAction::ATTACK_BOW;
                handled = true;
                break;
            case SDLK_6:
                m_actionName = CharaAction::CAST;
                handled = true;
                break;
            default:
                break;
        }
    } 
    else if (e.type == SDL_KEYUP) {
        // 当移动键释放时，回到站立状态
        if ((e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_s || 
                e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_d ||
                e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_DOWN ||
                e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT ||
                e.key.keysym.sym == SDLK_KP_8 || e.key.keysym.sym == SDLK_KP_2 ||
                e.key.keysym.sym == SDLK_KP_4 || e.key.keysym.sym == SDLK_KP_6) &&
            m_actionName == CharaAction::WALK) {
                m_actionName = CharaAction::STAND;
                handled = true;
        }
    }
    (void)lastFrameTime;
    (void)deltaTime;
    return handled;
}

SDL_Rect Persona::previewBounds(const CharaAction& actionName, const CharaDirection& direction) const
{
    bool initialized = false;
    SDL_Rect bounds{0, 0, 64, 64};

    for (const PartBase& partSprite : m_spriteParts)
    {
        SDL_Rect rect = partSprite.renderRect(actionName, direction, 0, 0);
        if (!initialized)
        {
            bounds = rect;
            initialized = true;
            continue;
        }

        const int left = std::min(bounds.x, rect.x);
        const int top = std::min(bounds.y, rect.y);
        const int right = std::max(bounds.x + bounds.w, rect.x + rect.w);
        const int bottom = std::max(bounds.y + bounds.h, rect.y + rect.h);
        bounds.x = left;
        bounds.y = top;
        bounds.w = right - left;
        bounds.h = bottom - top;
    }

    return bounds;
}

void Persona::tick(Uint32 deltaTime)
{
    update(m_actionName, m_direction, deltaTime);
}

void Persona::setPosition(int x, int y)
{
    m_x = x;
    m_y = y;
}

void Persona::setState(const CharaAction& actionName, const CharaDirection& direction, int x, int y)
{
    const bool stateChanged = m_actionName != actionName || m_direction != direction;
    m_actionName = actionName;
    m_direction = direction;
    m_x = x;
    m_y = y;

    if (!stateChanged) {
        return;
    }

    for (PartBase& partSprite : m_spriteParts)
    {
        partSprite.reset(m_actionName, m_direction);
    }
}

int Persona::x() const
{
    return m_x;
}

int Persona::y() const
{
    return m_y;
}

CharaAction Persona::action() const
{
    return m_actionName;
}

CharaDirection Persona::direction() const
{
    return m_direction;
}
