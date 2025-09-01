#pragma once 

#include "interfaceStruct.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class InterfaceManager
{
public:

    InterfaceManager();
    ~InterfaceManager();
    static InterfaceManager& getInstance();

    bool initWindow();
    void closeWindow();


    SDL_Renderer* renderer() const;

    // 刪除左值和右值  拷貝构造和赋值
    InterfaceManager(const InterfaceManager&) = delete;
    InterfaceManager(const InterfaceManager&&) = delete;
    InterfaceManager& operator=(const InterfaceManager&) = delete;
    InterfaceManager&& operator=(InterfaceManager) = delete;

private:
    Scene m_currentScene;
    SDL_Window* window;
    SDL_Renderer* renderer;
};