#pragma once 

#include "gameInterface.hpp"

// SettingScene 表示客户端设置页面的场景壳。
class SettingScene : public GameInterface
{
public:
    // 渲染设置场景内容。
    void renderScene() override;
    // 处理设置场景输入。
    void handleEvent(const SDL_Event &e) override;
    // 初始化设置场景资源。
    void initScene() override;
};
