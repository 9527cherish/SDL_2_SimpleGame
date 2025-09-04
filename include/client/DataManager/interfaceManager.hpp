#pragma once 

#include "sceneStruct.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "mainScene.hpp"
#include "settingScene.hpp"
#include "gameScene.hpp"
#include "gameInterface.hpp"

class InterfaceManager
{
public:

    InterfaceManager();
    ~InterfaceManager();
    static InterfaceManager& getInstance();
    //初始化窗口
    bool initWindow();
    // 关闭窗口
    void closeWindow();
    //执行程序
    void start();

    //处理事件
    void handleEvent();

    Scene currentScene();
    // 获取渲染器
    SDL_Renderer* renderer() const;

    // 刪除左值和右值  拷貝构造和赋值
    InterfaceManager(const InterfaceManager&) = delete;
    InterfaceManager(const InterfaceManager&&) = delete;
    InterfaceManager& operator=(const InterfaceManager&) = delete;
    InterfaceManager&& operator=(InterfaceManager) = delete;

private:
    // 当前所在场景
    Scene m_currentScene;
    // 渲染
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    // 不同场景
    MainScene* m_mainScene;
    SettingScene* m_settingScene;
    GameScene* m_gameScene;
};