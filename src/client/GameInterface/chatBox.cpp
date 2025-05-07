#include "chatBox.hpp"
#include "clientComonFunc.hpp"

ChatBox::ChatBox(SDL_Renderer *renderer, int screenWidth, int screenHeight)
    :m_renderer(renderer)
{

    if (TTF_Init() == -1) {
        spdlog::error("TTF_Init 初始化失败:" + std::string(TTF_GetError()));

    }
    m_font = TTF_OpenFont("arial.ttf", 20);
    if (!m_font) {
        spdlog::error("加载字体失败:"  + std::string(TTF_GetError()));
    }

    // 初始化聊天框位置（左下角）
    m_chatRect = {
        CHAT_MARGIN, 
        screenHeight - CHAT_HEIGHT - CHAT_MARGIN,
        CHAT_WIDTH,
        CHAT_HEIGHT
    };

    // 输入框位置（聊天框底部）
    m_inputRect = {
        m_chatRect.x + CHAT_MARGIN,
        m_chatRect.y + m_chatRect.h - 30,  // 假设输入框高度30px
        CHAT_WIDTH - 2*CHAT_MARGIN,
        25
    };
}

ChatBox::~ChatBox()
{
    for (auto& msg : m_messages) {
        if (msg.texture) {
            SDL_DestroyTexture(msg.texture);
        }
    }
    
    // 关闭字体
    if (m_font) {
        TTF_CloseFont(m_font);
    }
}

void ChatBox::handleEvent(SDL_Event *e)
{
    switch (e->type) {
        case SDL_MOUSEBUTTONDOWN: {
            // 检查是否点击了聊天框区域
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            SDL_Point mousePoint = {mouseX, mouseY};
            if (SDL_PointInRect(&mousePoint, &m_chatRect)) {
                m_isActive = true;
                SDL_StartTextInput();  // 开启文本输入
            } else {
                m_isActive = false;
                SDL_StopTextInput();   // 关闭文本输入
            }
            break;
        }

        case SDL_TEXTINPUT: {
            if (m_isActive) {
                m_inputText += e->text.text;  // 追加输入字符
            }
            break;
        }

        case SDL_KEYDOWN: {
            if (!m_isActive) break;

            switch (e->key.keysym.sym) {
                case SDLK_BACKSPACE:  // 退格删除
                    if (!m_inputText.empty()) {
                        m_inputText.pop_back();
                    }
                    break;

                case SDLK_RETURN:     // 回车提交
                    if (!m_inputText.empty()) {
                        addMessage(m_inputText);
                        m_inputText.clear();
                    }
                    break;
            }
            break;
        }
    }
}

void ChatBox::addMessage(const std::string &text)
{
    SDL_Surface* surface = TTF_RenderUTF8_Blended(m_font, text.c_str(), TEXT_COLOR);
    if (!surface) {
        SDL_Log("Failed to render text: %s", TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    if (!texture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    // 添加到消息列表
    m_messages.push_back({
        text, 
        texture, 
        surface->w, 
        surface->h
    });

    // 限制最大消息数量（例如保留最后10条）
    const int MAX_MESSAGES = 10;
    if (m_messages.size() > MAX_MESSAGES) {
        SDL_DestroyTexture(m_messages.front().texture);
        m_messages.erase(m_messages.begin());
    }

    SDL_FreeSurface(surface);
}

void ChatBox::render()
{
    // 绘制半透明背景
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(m_renderer, 
        BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, BG_COLOR.a);
    SDL_RenderFillRect(m_renderer, &m_chatRect);

    // 渲染历史消息（从底部向上）
    int yPos = m_chatRect.y + m_chatRect.h - 40;  // 初始Y位置（底部向上）
    for (auto it = m_messages.rbegin(); it != m_messages.rend(); ++it) {
        const auto& msg = *it;
        SDL_Rect destRect = {
            m_chatRect.x + CHAT_MARGIN,
            yPos - msg.height,
            msg.width,
            msg.height
        };

        // 超出聊天框顶部则停止渲染
        if (destRect.y < m_chatRect.y) break;

        SDL_RenderCopy(m_renderer, msg.texture, nullptr, &destRect);
        yPos -= msg.height + 5;  // 行间距5px
    }

    // 渲染输入框
    if (m_isActive) {
        // 输入文本 + 光标
        std::string inputText = "> " + m_inputText + "|";
        SDL_Surface* inputSurface = TTF_RenderUTF8_Blended(m_font, 
            inputText.c_str(), TEXT_COLOR);
        if (inputSurface) {
            SDL_Texture* inputTexture = SDL_CreateTextureFromSurface(
                m_renderer, inputSurface);
            SDL_Rect destRect = {
                m_inputRect.x,
                m_inputRect.y,
                std::min(inputSurface->w, m_inputRect.w),  // 防止溢出
                inputSurface->h
            };
            SDL_RenderCopy(m_renderer, inputTexture, nullptr, &destRect);
            SDL_DestroyTexture(inputTexture);
            SDL_FreeSurface(inputSurface);
        }
    }
}
