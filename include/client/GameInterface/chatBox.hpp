#pragma once 

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

const int CHAT_WIDTH = 300;
const int CHAT_HEIGHT = 200;

const int CHAT_MARGIN = 5;
// const int MAX_MESSAGES = 10;
const SDL_Color TEXT_COLOR = {255, 255, 255, 255};
const SDL_Color BG_COLOR = {0, 0, 0, 150};  // r g b a


struct ChatMessage {
    std::string text;
    SDL_Texture* texture = nullptr;
    int width = 0;
    int height = 0;
};


class ChatBox{

public:
    ChatBox(SDL_Renderer* renderer, int screenWidth, int screenHeight);
    ~ChatBox();
    
    // 处理聊天框键盘事件
    void handleEvent(SDL_Event* e);
    // 添加消息
    void addMessage(const std::string& text);
    // 渲染聊天框
    void render();


private:
    SDL_Renderer* m_renderer;
    TTF_Font* m_font;
    std::vector<ChatMessage> m_messages;
    std::string m_inputText;
    SDL_Rect m_chatRect;
    SDL_Rect m_inputRect;
    bool m_isActive = false;
};
