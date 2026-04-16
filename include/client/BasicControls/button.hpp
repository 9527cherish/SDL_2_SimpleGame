#pragma once 


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "clientComonFunc.hpp"
#include "sceneStruct.hpp"


// 按钮状态
enum class ButtonState {
    NORMAL,
    HOVER,
    PRESSED
};

// Button 封装了菜单按钮的文本、外观状态和鼠标交互行为
class Button
{

public:
    Button(SDL_Renderer* renderer, TTF_Font* font, const std::string& text
                , int x, int y, int w, int h, SDL_Color normal
                , SDL_Color hover, SDL_Color pressed, SDL_Color textColor);
    ~Button();
    // 渲染当前按钮外观与文本
    void renderButton();
    // 处理鼠标事件并在点击成功时返回 true
    bool handleEvent(const SDL_Event& e);
    // 设置按钮是否可见
    void setVisible(bool v);
    // 返回按钮当前是否可见
    bool isVisible() const;
    // 更新按钮显示文本
    void setText(const std::string& t);
    // 直接设置按钮文本纹理
    void setTexture(SDL_Texture* texture);
    // 更新按钮使用的字体对象
    void setFont(TTF_Font* font);
    // 设置是否绘制按钮边框
    void setBorder(bool flag);

private:
    // 按钮渲染所使用的 SDL 渲染器
    SDL_Renderer* m_pRenderer = nullptr;
    // 按钮文本所使用的字体对象
    TTF_Font* m_pFont = nullptr;
    // 按钮当前显示的文本内容
    std::string m_text;
    // 按钮在屏幕上的区域
    SDL_Rect m_rect;

    // 普通状态下的背景颜色
    SDL_Color m_normalColor;
    // 鼠标悬停状态下的背景颜色
    SDL_Color m_hoverColor;
    // 鼠标按下状态下的背景颜色
    SDL_Color m_pressedColor;
    // 按钮文本颜色
    SDL_Color m_textColor;
    // 按钮当前交互状态
    ButtonState m_state = ButtonState::NORMAL;
    // 按钮是否参与渲染和事件处理
    bool m_visible = true;
    // 缓存后的按钮文本纹理
    SDL_Texture* m_pTextTexture = nullptr;

    // 是否绘制按钮边框
    bool m_bRenderBorder = true;

    // 根据当前文本和字体刷新文本纹理缓存
    void updateTextTexture();
};
