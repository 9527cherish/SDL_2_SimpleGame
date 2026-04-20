#pragma once 
#include "partBase.hpp"
#include "sceneStruct.hpp"
#include <string>

// Persona 表示本机可输入、可更新、可渲染的完整角色实例
class Persona
{
public:
    Persona() = default;
    // 自定义拷贝构造，实现深拷贝
    Persona(const Persona& persona);

    // 返回角色模板或实例 ID
    uint id() const;
    // 设置角色模板或实例 ID
    void setId(uint id);
    // 返回角色包含的所有部件。
    const std::vector<PartBase>& spriteParts() const;
    // 在预览场景中渲染角色。
    void renderer(SDL_Renderer* renderer, int x, int y);
    // 以原始尺寸渲染当前角色。
    void rendererCurPersona(SDL_Renderer* renderer, int x, int y);
    // 按指定缩放比例渲染当前角色。
    void rendererCurPersonaScaled(SDL_Renderer* renderer, int x, int y, float scale);
    // 在游戏场景中按固定基准盒渲染当前角色。
    void rendererCurPersonaGameScaled(SDL_Renderer* renderer, int x, int y, float scale);
    // 以脚点为锚点按比例渲染当前角色。
    void rendererCurPersonaFootScaled(SDL_Renderer* renderer, int footX, int footY, float scale);
    // 以默认位置按比例渲染当前角色。
    void rendererCurPersonaScaled(SDL_Renderer* renderer, float scale);
    // 以默认位置渲染当前角色。
    void rendererCurPersona(SDL_Renderer* renderer);
    // 推进角色下所有部件的动画状态。
    void update(const CharaAction& actionName, const CharaDirection& direction, int deltaTime);
    // 给角色添加一个部件。
    void addPartBase(const PartBase& part);
    // 打印角色当前的调试信息。
    void printPersonaInfo();
    // 处理角色输入并更新动作状态。
    bool handleEvent(const SDL_Event& e, Uint32& lastFrameTime, Uint32& deltaTime, bool move = true);
    // 按时间推进角色移动与动作。
    bool tick(Uint32 deltaTime);
    // 直接设置角色世界坐标。
    void setPosition(int x, int y);
    // 直接覆盖角色的动作、方向和位置状态。
    void setState(const CharaAction& actionName, const CharaDirection& direction, int x, int y);
    // 返回角色世界坐标 X。
    int x() const;
    // 返回角色世界坐标 Y。
    int y() const;
    // 返回角色当前动作。
    CharaAction action() const;
    // 返回角色当前方向。
    CharaDirection direction() const;
    // 触发一次本地动作播放。
    void playAction(const CharaAction& actionName);
    // 导出角色所有部件的同步信息
    std::vector<PartSyncInfo> partSyncInfos() const;

private:
    // 计算预览渲染时角色整体包围盒。
    SDL_Rect previewBounds(const CharaAction& actionName, const CharaDirection& direction) const;
    // 重置角色当前动画状态。
    void resetAnimationState();
    // 触发一次性动作播放。
    void triggerAction(const CharaAction& actionName);
    // 返回指定动作的预计持续时间。
    Uint32 actionDuration(const CharaAction& actionName) const;
    // 根据输入状态更新基础动作。
    void updateBaseAction();
    // 记录某个移动按键的按下或释放状态。
    void setMoveKey(SDL_Keycode key, bool pressed, bool& handled);
    // 判断角色当前是否有移动输入。
    bool hasMoveInput() const;

    // 角色模板或实例 ID
    uint m_id = 0;
    // 角色包含的所有精灵部件
    std::vector<PartBase> m_spriteParts;

    // 角色当前世界坐标 X
    int m_x = SCREEN_WIDTH/2;
    // 角色当前世界坐标 Y
    int m_y = SCREEN_HEIGHT/2;
    // 角色高精度世界坐标 X
    float m_preciseX = static_cast<float>(SCREEN_WIDTH/2);
    // 角色高精度世界坐标 Y
    float m_preciseY = static_cast<float>(SCREEN_WIDTH/2);
    // 角色当前动作
    CharaAction m_actionName = CharaAction::STAND;
    // 无输入时应回到的基础动作
    CharaAction m_idleAction = CharaAction::STAND;
    // 角色当前面朝方向
    CharaDirection m_direction = CharaDirection::DOWN;
    // 是否正在向上移动
    bool m_moveUp = false;
    // 是否正在向下移动
    bool m_moveDown = false;
    // 是否正在向左移动
    bool m_moveLeft = false;
    // 是否正在向右移动
    bool m_moveRight = false;
    // 是否允许在 tick 中处理持续移动
    bool m_enableMoveByTick = true;
    // 当前是否处于不可被基础动作打断的动作锁定状态
    bool m_actionLocked = false;
    // 当前锁定动作剩余的播放时间
    Uint32 m_actionRemainTime = 0;
    // 角色每秒移动速度
    float m_moveSpeed = 220.0f;
};
