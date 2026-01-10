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


class Button
{

public:
    Button(SDL_Renderer* renderer, TTF_Font* font, const std::string& text
                , int x, int y, int w, int h, SDL_Color normal
                , SDL_Color hover, SDL_Color pressed, SDL_Color textColor);
    ~Button();
    void renderButton();
    bool handleEvent(const SDL_Event& e);
    void setVisible(bool v);
    bool isVisible() const;
    void setText(const std::string& t);
    void setTexture(SDL_Texture* texture);
    void setFont(TTF_Font* font);
    void setBorder(bool flag);

private:
    SDL_Renderer* m_pRenderer;
    TTF_Font* m_pFont;
    std::string m_text;
    SDL_Rect m_rect;

    SDL_Color m_normalColor;
    SDL_Color m_hoverColor;
    SDL_Color m_pressedColor;
    SDL_Color m_textColor;
    ButtonState m_state;
    bool m_visible;
    SDL_Texture* m_pTextTexture;

    bool m_bRenderBorder;

    void updateTextTexture();
};

