#pragma once 

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <mutex>

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

    // 获取最新发送的消息
    std::string generateSendMsg();

    // 设置需要发送数据的值
    void setSendText(const std::string& msg);

private:
    SDL_Renderer* m_renderer;
    TTF_Font* m_font;

    std::vector<ChatMessage> m_messages;
    std::mutex m_csMsg;
    // 记录当前输入的信息
    std::string m_inputText;
    // 记录需要发送的数据
    std::string m_sendText;
    SDL_Rect m_chatRect;
    SDL_Rect m_inputRect;
    bool m_isActive = false;
};

extern  ChatBox* g_chatBox;
