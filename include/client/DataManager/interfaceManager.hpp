#pragma once 

#include "characterStruct.hpp"


class InterfaceManager
{
public:
    static InterfaceManager& getInstance();


    // 刪除左值和右值  拷貝构造和赋值
    InterfaceManager(const InterfaceManager&) = delete;
    InterfaceManager(const InterfaceManager&&) = delete;
    InterfaceManager& operator=(const InterfaceManager&) = delete;
    InterfaceManager&& operator=(InterfaceManager) = delete;

private:
    Scene m_currentScene;
};