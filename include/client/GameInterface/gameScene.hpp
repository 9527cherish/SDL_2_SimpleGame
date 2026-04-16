#pragma once 

#include "gameInterface.hpp"
#include "worldScenePresenter.hpp"

// GameScene 表示游戏内场景壳，实际世界渲染委托给 WorldScenePresenter
class GameScene : public GameInterface
{
public:
    GameScene();
    ~GameScene() override;

    // 渲染当前游戏场景。
    void renderScene() override;
    // 处理游戏场景输入。
    void handleEvent(const SDL_Event &e) override;
    // 初始化游戏场景资源。
    void initScene() override;
private:
    // 当前场景使用的渲染器
    SDL_Renderer* m_pRenderer = nullptr;
    // 实际负责世界渲染与交互的 presenter
    WorldScenePresenter m_worldPresenter;
};
