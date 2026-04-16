#pragma once

#include <SDL2/SDL.h>
#include "camera.hpp"

// WorldScenePresenter 负责游戏世界的渲染组织、相机跟随与联机交互驱动
class WorldScenePresenter
{
public:
    WorldScenePresenter();
    ~WorldScenePresenter();

    // 初始化世界渲染所需资源。
    void init(SDL_Renderer* renderer);
    // 渲染当前游戏世界。
    void render();
    // 处理世界场景中的输入事件。
    void handleEvent(const SDL_Event& e);

private:
    // 渲染本机角色。
    void renderCurrentPerson();
    // 渲染背景大图。
    void renderBackground();
    // 渲染所有远端角色。
    void renderRemotePersons(Uint32 deltaTime);

private:
    // 世界渲染使用的渲染器
    SDL_Renderer* m_renderer = nullptr;
    // 背景大图纹理
    SDL_Texture* m_backgroundTexture = nullptr;
    // 背景纹理宽度
    int m_backgroundWidth = 0;
    // 背景纹理高度
    int m_backgroundHeight = 0;
    // 当前场景相机
    Camera m_camera;
    // 上一帧的时间戳
    Uint32 m_lastFrameTime = 0;
};
