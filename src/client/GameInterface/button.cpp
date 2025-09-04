#include "button.hpp"

Button::Button(SDL_Renderer *renderer, TTF_Font *font, const std::string &text, int x, int y
                , int w, int h, SDL_Color normal, SDL_Color hover, SDL_Color pressed, SDL_Color textColor)
                : renderer(renderer), font(font), text(text) 
                , rect({x, y, w, h})
                , normalColor(normal), hoverColor(hover), pressedColor(pressed), textColor(textColor)
                , state(ButtonState::NORMAL), visible(true), textTexture(nullptr)
                
{
    updateTextTexture();
}

Button::~Button()
{
    if (textTexture){
        SDL_DestroyTexture(textTexture);
     }
}

void Button::renderButton()
{
    if (!visible) return;

    // 绘制按钮背景
    SDL_Color currentColor;
    switch (state) {
        case ButtonState::NORMAL: currentColor = normalColor; break;
        case ButtonState::HOVER: currentColor = hoverColor; break;
        case ButtonState::PRESSED: currentColor = pressedColor; break;
    }

    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    SDL_RenderFillRect(renderer, &rect);

    // 绘制按钮边框
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
    SDL_RenderDrawRect(renderer, &rect);

    // 绘制文本
    if (textTexture) {
        int textW, textH;
        SDL_QueryTexture(textTexture, nullptr, nullptr, &textW, &textH);
        SDL_Rect textRect = {
            rect.x + (rect.w - textW) / 2,
            rect.y + (rect.h - textH) / 2,
            textW, textH
        };
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    }
}

bool Button::handleEvent(const SDL_Event &e)
{
    if (!visible) return false;

    int x, y;
    SDL_GetMouseState(&x, &y);

    bool inside = (x >= rect.x && x < rect.x + rect.w &&
                    y >= rect.y && y < rect.y + rect.h);

    if (!inside) {
        state = ButtonState::NORMAL;
        return false;
    }

    switch (e.type) {
        case SDL_MOUSEMOTION:
            if (state != ButtonState::PRESSED) {
                state = ButtonState::HOVER;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_LEFT) {
                state = ButtonState::PRESSED;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (e.button.button == SDL_BUTTON_LEFT && state == ButtonState::PRESSED) {
                state = ButtonState::HOVER;
                return true; // 按钮被点击
            }
            break;
    }
    return false;
}

void Button::setVisible(bool v)
{
    visible = v;
}

bool Button::isVisible() const
{
    return visible;
}

void Button::setText(const std::string &t)
{
    text = t; 
    updateTextTexture();  
}

void Button::updateTextTexture()
{
    if (textTexture) {
        SDL_DestroyTexture(textTexture);
    }

    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    if (!textSurface) {
        // std::cerr << "无法创建文本表面: " << TTF_GetError() << std::endl;
        return;
    }

    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
}
