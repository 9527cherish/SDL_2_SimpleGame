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

private:
    SDL_Rect rect;
    std::string text;
    TTF_Font* font;
    SDL_Color normalColor;
    SDL_Color hoverColor;
    SDL_Color pressedColor;
    SDL_Color textColor;
    ButtonState state;
    bool visible;
    SDL_Texture* textTexture;
    SDL_Renderer* renderer;

    void updateTextTexture();
};

