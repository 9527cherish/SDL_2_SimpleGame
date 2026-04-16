#pragma once 

#include "sceneStruct.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "mainScene.hpp"
#include "settingScene.hpp"
#include "gameScene.hpp"
#include "gameInterface.hpp"
#include "cursor.hpp"

// InterfaceManager 负责客户端窗口生命周期、场景切换和主循环调度
class InterfaceManager
{
public:
    ~InterfaceManager();
    // 返回界面管理器的全局实例。
    static InterfaceManager& getInstance();
    // 初始化 SDL 窗口与渲染器。
    bool initWindow();
    // 关闭窗口并释放 SDL 资源。
    void closeWindow();
    // 进入客户端主循环。
    void start();

    // 把输入事件分发给当前场景。
    void handleEvent(const SDL_Event &e);
    // 渲染当前处于激活状态的场景。
    void renderCurrentScene();

    // 返回当前场景枚举值。
    Scene currentScene();
    // 切换当前激活场景。
    void  setCurrentScene(const Scene& scene);
    // 返回当前窗口使用的渲染器。
    SDL_Renderer* renderer() const;
private:
    // 当前处于激活状态的场景
    Scene m_currentScene = Scene::MAIN_MENU;
    // SDL 主窗口对象
    SDL_Window* m_pWindow = nullptr;
    // 主窗口共用渲染器
    SDL_Renderer* m_pRenderer = nullptr;
    // 主菜单场景实例
    std::unique_ptr<MainScene> m_pMainScene;
    // 设置场景实例
    std::unique_ptr<SettingScene> m_pSettingScene;
    // 游戏场景实例
    std::unique_ptr<GameScene> m_pGameScene;
    // 自定义鼠标控制器
    std::unique_ptr<Cursor> m_pCursor;

    InterfaceManager();
    InterfaceManager(const InterfaceManager&) = delete;
    InterfaceManager(const InterfaceManager&&) = delete;
    InterfaceManager& operator=(const InterfaceManager&) = delete;
    InterfaceManager& operator=(InterfaceManager&&) = delete;

};
