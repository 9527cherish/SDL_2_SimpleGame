#pragma once 

#include "sceneStruct.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "mainScene.hpp"
#include "settingScene.hpp"
#include "gameScene.hpp"
#include "gameInterface.hpp"
#include "cursor.hpp"

class InterfaceManager
{
public:
    ~InterfaceManager();
    static InterfaceManager& getInstance();
    //初始化窗口
    bool initWindow();
    // 关闭窗口
    void closeWindow();
    //执行程序
    void start();

    //处理事件
    void handleEvent(const SDL_Event &e);
    void renderCurrentScene();

    Scene currentScene();
    void  setCurrentScene(const Scene& scene);
    // 获取渲染器
    SDL_Renderer* renderer() const;
private:
    // 当前所在场景
    Scene m_currentScene;
    // 渲染
    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;
    // 不同场景
    std::unique_ptr<MainScene> m_pMainScene;
    std::unique_ptr<SettingScene> m_pSettingScene;
    std::unique_ptr<GameScene> m_pGameScene;
    std::unique_ptr<Cursor> m_pCursor;

    InterfaceManager();
    // 刪除左值和右值  拷貝构造和赋值
    InterfaceManager(const InterfaceManager&) = delete;
    InterfaceManager(const InterfaceManager&&) = delete;
    InterfaceManager& operator=(const InterfaceManager&) = delete;
    InterfaceManager& operator=(InterfaceManager&&) = delete;

};
