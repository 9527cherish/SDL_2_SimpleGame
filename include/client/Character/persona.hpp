#pragma once 
#include "partBase.hpp"
#include "sceneStruct.hpp"


class Persona
{
public:
    Persona() = default;
    // 自定义拷贝构造，实现深拷贝
    Persona(const Persona& persona);

    void setId(uint id);
    void renderer(SDL_Renderer* renderer, int x, int y);
    void rendererCurPersona(SDL_Renderer* renderer, int x, int y);
    void rendererCurPersonaScaled(SDL_Renderer* renderer, int x, int y, float scale);
    void rendererCurPersona(SDL_Renderer* renderer);
    void update(const CharaAction& actionName, const CharaDirection& direction, int deltaTime);
    void addPartBase(const PartBase& part);
    void printPersonaInfo();
    bool handleEvent(const SDL_Event& e, Uint32& lastFrameTime, Uint32& deltaTime, bool move = true);

private:
    SDL_Rect previewBounds(const CharaAction& actionName, const CharaDirection& direction) const;

    uint m_id = 0;                         // NPC ID
    std::vector<PartBase> m_spriteParts;  // 精灵部件列表

    int m_x = SCREEN_WIDTH/2;                     // 角色的X坐标
    int m_y = SCREEN_HEIGHT/2;                     // 角色的Y坐标
    // 人物在未收到输入前也要有稳定的初始状态，否则预览界面会使用未定义动作/方向取帧。
    CharaAction m_actionName = CharaAction::STAND;
    CharaDirection m_direction = CharaDirection::DOWN;
};
