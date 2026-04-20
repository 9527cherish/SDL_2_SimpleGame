#include "persona.hpp"
#include "characterStruct.hpp"
#include "loadXml.hpp"
#include "Collision/worldCollisionManager.hpp"
#include <algorithm>
#include <cmath>

Persona::Persona(const Persona &persona)
{
    m_id = persona.m_id;
    m_spriteParts = persona.m_spriteParts; // std::vector has its own copy constructor
    m_x = persona.m_x;
    m_y = persona.m_y;
    m_preciseX = persona.m_preciseX;
    m_preciseY = persona.m_preciseY;
    m_actionName = persona.m_actionName;
    m_idleAction = persona.m_idleAction;
    m_direction = persona.m_direction;
    m_moveUp = persona.m_moveUp;
    m_moveDown = persona.m_moveDown;
    m_moveLeft = persona.m_moveLeft;
    m_moveRight = persona.m_moveRight;
    m_enableMoveByTick = persona.m_enableMoveByTick;
    m_actionLocked = persona.m_actionLocked;
    m_actionRemainTime = persona.m_actionRemainTime;
    m_moveSpeed = persona.m_moveSpeed;
}

uint Persona::id() const
{
    return m_id;
}

const std::vector<PartBase>& Persona::spriteParts() const
{
    return m_spriteParts;
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

void Persona::rendererCurPersonaFootScaled(SDL_Renderer* renderer, int footX, int footY, float scale)
{
    int drawX = static_cast<int>(footX - 32.0f * scale);
    int drawY = static_cast<int>(footY - 64.0f * scale);

    for (PartBase& partSprite : m_spriteParts)
    {
        partSprite.initTexture(renderer);
        partSprite.renderScaled(renderer, m_actionName, m_direction, drawX, drawY, scale);
    }
}

void Persona::rendererCurPersonaGameScaled(SDL_Renderer *renderer, int x, int y, float scale)
{
    // 游戏场景里统一以 64x64 人物基准盒的中心为锚点，避免不同动画帧 offset 改变时看起来整个人在漂移。
    int drawX = static_cast<int>(x - 32.0f * scale);
    int drawY = static_cast<int>(y - 32.0f * scale);

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
    // 游戏场景下由 tick 按“当前按键状态”持续移动；角色预览场景只切动作不改坐标。
    m_enableMoveByTick = move;

    if(e.type == SDL_KEYDOWN) {
        setMoveKey(e.key.keysym.sym, true, handled);
        if (handled && move && !e.key.repeat)
        {
            m_idleAction = CharaAction::STAND;
        }

        if (e.key.repeat) {
            (void)lastFrameTime;
            (void)deltaTime;
            return handled;
        }

        switch (e.key.keysym.sym) {
            case SDLK_w:
            case SDLK_UP:
            case SDLK_KP_8:
                m_direction = CharaDirection::UP;
                break;
            case SDLK_s:
            case SDLK_DOWN:
            case SDLK_KP_2:
                m_direction = CharaDirection::DOWN;
                break;
            case SDLK_a:
            case SDLK_LEFT:
            case SDLK_KP_4:
                m_direction = CharaDirection::LEFT;
                break;
            case SDLK_d:
            case SDLK_RIGHT:
            case SDLK_KP_6:
                m_direction = CharaDirection::RIGHT;
                break;
            case SDLK_SPACE:
                triggerAction(CharaAction::ATTACK);
                handled = true;
                break;
            case SDLK_1:
                m_idleAction = CharaAction::STAND;
                if (!m_actionLocked && !hasMoveInput()) {
                    m_actionName = m_idleAction;
                    resetAnimationState();
                }
                handled = true;
                break;
            case SDLK_2:
                m_idleAction = CharaAction::SIT;
                if (!m_actionLocked && !hasMoveInput()) {
                    m_actionName = m_idleAction;
                    resetAnimationState();
                }
                handled = true;
                break;
            case SDLK_3:
                m_idleAction = CharaAction::DEAD;
                if (!m_actionLocked && !hasMoveInput()) {
                    m_actionName = m_idleAction;
                    resetAnimationState();
                }
                handled = true;
                break;
            case SDLK_4:
                triggerAction(CharaAction::ATTACK_SWORD_STAB);
                handled = true;
                break;
            case SDLK_5:
                triggerAction(CharaAction::ATTACK_BOW);
                handled = true;
                break;
            case SDLK_6:
                triggerAction(CharaAction::ATTACK_SPEAR);
                handled = true;
                break;
            case SDLK_7:
                triggerAction(CharaAction::ATTACK_CHOP);
                handled = true;
                break;
            case SDLK_8:
                triggerAction(CharaAction::ATTACK_2HAND);
                handled = true;
                break;
            case SDLK_9:
            case SDLK_c:
                triggerAction(CharaAction::CAST);
                handled = true;
                break;
            default:
                break;
        }
    } 
    else if (e.type == SDL_KEYUP) {
        setMoveKey(e.key.keysym.sym, false, handled);
        if (handled && !m_actionLocked)
        {
            updateBaseAction();
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

bool Persona::tick(Uint32 deltaTime)
{
    const int oldX = m_x;
    const int oldY = m_y;
    const CharaAction oldAction = m_actionName;
    const CharaDirection oldDirection = m_direction;

    if (m_actionLocked)
    {
        // 攻击/施法这类一次性动作在锁定期内优先播放，避免被移动输入立刻打断。
        if (deltaTime >= m_actionRemainTime) {
            m_actionRemainTime = 0;
            m_actionLocked = false;
        } else {
            m_actionRemainTime -= deltaTime;
        }
    }

    if (m_enableMoveByTick && !m_actionLocked && hasMoveInput())
    {
        float offsetX = 0.0f;
        float offsetY = 0.0f;
        // 使用 deltaTime 计算位移，避免移动速度依赖键盘重复率或帧率波动。
        const float step = m_moveSpeed * static_cast<float>(deltaTime) / 1000.0f;

        if (m_moveUp) {
            offsetY -= step;
            m_direction = CharaDirection::UP;
        }
        if (m_moveDown) {
            offsetY += step;
            m_direction = CharaDirection::DOWN;
        }
        if (m_moveLeft) {
            offsetX -= step;
            m_direction = CharaDirection::LEFT;
        }
        if (m_moveRight) {
            offsetX += step;
            m_direction = CharaDirection::RIGHT;
        }

        const CharaAction previousAction = m_actionName;
        const float nextPreciseX = m_preciseX + offsetX;
        const float nextPreciseY = m_preciseY + offsetY;
        const int nextX = static_cast<int>(std::lround(nextPreciseX));
        const int nextY = static_cast<int>(std::lround(nextPreciseY));
        if (WorldCollisionManager::getInstance().canOccupyPlayerFootbox(nextX, nextY))
        {
            m_preciseX = nextPreciseX;
            m_preciseY = nextPreciseY;
            m_x = nextX;
            m_y = nextY;
        }
        m_actionName = CharaAction::WALK;
        if (previousAction != m_actionName) {
            resetAnimationState();
        }
    }
    else if (!m_actionLocked)
    {
        updateBaseAction();
    }

    update(m_actionName, m_direction, deltaTime);
    return oldX != m_x || oldY != m_y || oldAction != m_actionName || oldDirection != m_direction;
}

void Persona::setPosition(int x, int y)
{
    m_x = x;
    m_y = y;
    m_preciseX = static_cast<float>(x);
    m_preciseY = static_cast<float>(y);
}

void Persona::setState(const CharaAction& actionName, const CharaDirection& direction, int x, int y)
{
    const bool stateChanged = m_actionName != actionName || m_direction != direction;
    m_actionName = actionName;
    m_direction = direction;
    m_x = x;
    m_y = y;
    m_preciseX = static_cast<float>(x);
    m_preciseY = static_cast<float>(y);
    m_idleAction = actionName == CharaAction::WALK ? CharaAction::STAND : actionName;
    m_actionLocked = false;
    m_actionRemainTime = 0;
    m_moveUp = false;
    m_moveDown = false;
    m_moveLeft = false;
    m_moveRight = false;

    if (!stateChanged) {
        return;
    }

    resetAnimationState();
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

void Persona::playAction(const CharaAction& actionName)
{
    triggerAction(actionName);
}

std::vector<PartSyncInfo> Persona::partSyncInfos() const
{
    std::vector<PartSyncInfo> infos;
    infos.reserve(m_spriteParts.size());
    for (const PartBase& partSprite : m_spriteParts)
    {
        infos.emplace_back(partSprite.syncInfo());
    }
    return infos;
}

void Persona::resetAnimationState()
{
    for (PartBase& partSprite : m_spriteParts)
    {
        partSprite.reset(m_actionName, m_direction);
    }
}

void Persona::triggerAction(const CharaAction& actionName)
{
    // 一次性动作进入锁定状态，播放完后再回到站立或行走。
    m_actionName = actionName;
    m_actionLocked = true;
    m_actionRemainTime = actionDuration(actionName);
    resetAnimationState();
}

Uint32 Persona::actionDuration(const CharaAction& actionName) const
{
    switch (actionName)
    {
    case CharaAction::ATTACK:
    case CharaAction::ATTACK_SWORD_STAB:
        return 420;
    case CharaAction::ATTACK_BOW:
        return 460;
    case CharaAction::ATTACK_SPEAR:
    case CharaAction::ATTACK_CHOP:
    case CharaAction::ATTACK_2HAND:
        return 560;
    case CharaAction::CAST:
        return 700;
    default:
        return 0;
    }
}

void Persona::updateBaseAction()
{
    if (hasMoveInput() && m_enableMoveByTick)
    {
        m_actionName = CharaAction::WALK;
        return;
    }

    if (m_actionName != m_idleAction)
    {
        m_actionName = m_idleAction;
        resetAnimationState();
    }
}

void Persona::setMoveKey(SDL_Keycode key, bool pressed, bool& handled)
{
    switch (key)
    {
    case SDLK_w:
    case SDLK_UP:
    case SDLK_KP_8:
        m_moveUp = pressed;
        if (pressed) {
            m_direction = CharaDirection::UP;
        }
        handled = true;
        break;
    case SDLK_s:
    case SDLK_DOWN:
    case SDLK_KP_2:
        m_moveDown = pressed;
        if (pressed) {
            m_direction = CharaDirection::DOWN;
        }
        handled = true;
        break;
    case SDLK_a:
    case SDLK_LEFT:
    case SDLK_KP_4:
        m_moveLeft = pressed;
        if (pressed) {
            m_direction = CharaDirection::LEFT;
        }
        handled = true;
        break;
    case SDLK_d:
    case SDLK_RIGHT:
    case SDLK_KP_6:
        m_moveRight = pressed;
        if (pressed) {
            m_direction = CharaDirection::RIGHT;
        }
        handled = true;
        break;
    default:
        break;
    }
}

bool Persona::hasMoveInput() const
{
    return m_moveUp || m_moveDown || m_moveLeft || m_moveRight;
}
