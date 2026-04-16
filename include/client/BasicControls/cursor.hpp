#pragma once 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <atomic>
#include "memory"

// Cursor 负责加载自定义鼠标样式并根据点击状态切换光标
class Cursor
{
public:
    Cursor();
    ~Cursor();
    // 释放当前创建的 SDL 光标资源
    void freeCursor();

    // 重新加载光标资源
    void reloadCursor();
    // 初始化普通和点击态光标
    void initCursor();
    // 根据鼠标事件切换当前显示的光标
    bool handleEvent(const SDL_Event& e); 

private:
    // 系统原始光标，用于恢复默认状态
    SDL_Cursor* m_pOriginalCursor = nullptr;
    // 普通状态下使用的自定义光标
    SDL_Cursor* m_pCursor = nullptr;
    // 鼠标按下时使用的自定义光标
    SDL_Cursor* m_pCursorClicked = nullptr;
};
