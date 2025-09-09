#include "button.hpp"

Button::Button(SDL_Renderer *renderer, TTF_Font *font, const std::string &text, int x, int y
                , int w, int h, SDL_Color normal, SDL_Color hover, SDL_Color pressed, SDL_Color textColor)
                : m_pRenderer(renderer), m_pFont(font), m_text(text) 
                , m_rect({x, y, w, h})
                , m_normalColor(normal), m_hoverColor(hover), m_pressedColor(pressed), m_textColor(textColor)
                , m_state(ButtonState::NORMAL), m_visible(true), m_pTextTexture(nullptr), m_bRenderBorder(true)
                
{
    if (TTF_Init() == -1) {
        spdlog::error("TTF_Init 初始化失败:" + std::string(TTF_GetError()));
    }

    m_pFont = TTF_OpenFont("SourceHanSansCN-Regular.otf", 24);
    if (!m_pFont) {
        spdlog::error("Button 加载字体失败:"  + std::string(TTF_GetError()));
    }

    updateTextTexture();
}

Button::~Button()
{
    if (m_pTextTexture){
        SDL_DestroyTexture(m_pTextTexture);
     }
}

void Button::renderButton()
{
    if (!m_visible) return;

    // 绘制按钮背景
    SDL_Color currentColor;
    switch (m_state) {
        case ButtonState::NORMAL: currentColor = m_normalColor; break;
        case ButtonState::HOVER: currentColor = m_hoverColor; break;
        case ButtonState::PRESSED: currentColor = m_pressedColor; break;
    }

    SDL_SetRenderDrawColor(m_pRenderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    SDL_RenderFillRect(m_pRenderer, &m_rect);

    // 绘制按钮边框
    if(m_bRenderBorder)
    {
        SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 200);
        SDL_RenderDrawRect(m_pRenderer, &m_rect);
    }

    // 绘制文本
    if (m_pTextTexture) {
        int textW, textH;
        SDL_QueryTexture(m_pTextTexture, nullptr, nullptr, &textW, &textH);
        SDL_Rect textRect = {
            m_rect.x + (m_rect.w - textW) / 2,
            m_rect.y + (m_rect.h - textH) / 2,
            textW, textH
        };
        SDL_RenderCopy(m_pRenderer, m_pTextTexture, nullptr, &textRect);
    }
}

bool Button::handleEvent(const SDL_Event &e)
{
    if (!m_visible) return false;

    int x, y;
    SDL_GetMouseState(&x, &y);

    bool inside = (x >= m_rect.x && x < m_rect.x + m_rect.w &&
                    y >= m_rect.y && y < m_rect.y + m_rect.h);

    if (!inside) {
        m_state = ButtonState::NORMAL;
        return false;
    }

    switch (e.type) {
        case SDL_MOUSEMOTION:
            if (m_state != ButtonState::PRESSED) {
                m_state = ButtonState::HOVER;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_LEFT) {
                m_state = ButtonState::PRESSED;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (e.button.button == SDL_BUTTON_LEFT && m_state == ButtonState::PRESSED) {
                m_state = ButtonState::HOVER;
                return true; // 按钮被点击
            }
            break;
    }
    return false;
}

void Button::setVisible(bool v)
{
    m_visible = v;
}

bool Button::isVisible() const
{
    return m_visible;
}

void Button::setText(const std::string &t)
{
    m_text = t; 
    updateTextTexture();  
}

void Button::setTexture(SDL_Texture *texture)
{
    m_pTextTexture = texture;
}

void Button::setFont(TTF_Font *font)
{
    m_pFont = font;
}

void Button::setBorder(bool flag)
{
    m_bRenderBorder = flag;
}

void Button::updateTextTexture()
{
    if (m_pTextTexture) {
        SDL_DestroyTexture(m_pTextTexture);
    }

    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(m_pFont, m_text.c_str(), m_textColor);
    if (!textSurface) {
        spdlog::error("无法创建文本表面: " + std::string(TTF_GetError()));
        return;
    }

    m_pTextTexture = SDL_CreateTextureFromSurface(m_pRenderer, textSurface);
    SDL_FreeSurface(textSurface);
}
