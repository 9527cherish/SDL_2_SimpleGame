#pragma once 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// GameInterface 是所有客户端场景的统一基类接口
class GameInterface
{
public:
    GameInterface();

    // 析构函数声明称虚函数，否则析构的时候只会调用基类的析构函数，派生类的无法调用
    virtual ~GameInterface();

    // 确保场景只初始化一次。
    void init();

    // 初始化场景自身资源。
    virtual void initScene() = 0;
    // 渲染场景内容。
    virtual void renderScene() = 0;
    // 处理场景事件输入。
    virtual void handleEvent(const SDL_Event &e) = 0;

    // 判断某个点是否位于指定矩形区域内。
    virtual bool isPointInRect(const int& x,const  int& y, const  SDL_Rect& rect);

protected:
    // 场景是否已经完成初始化
    bool m_binit;

};
