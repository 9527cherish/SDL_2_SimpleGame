#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.hpp"
#include "./HttpServer/clientServer.hpp"
#include "clientComonFunc.hpp"
#include "chatBox.hpp"



void startServerDataProcess(ClientServer* clientServer){
    std::thread t = std::thread(&ClientServer::dealServerData,  clientServer);
    t.detach();
}


/**
 *  后续重新封装main函数，太长，将一些功能进行封装
 *  在执行程序的时候提供传入姓名的功能
 */

int main(int argc, char* argv[]) {

    ClientComonFunc::getInstance();

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("SDL2 Player Animation",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* bgTexture = IMG_LoadTexture(renderer, "../images/map.png");

    g_player = new Player(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 5, renderer);

    ClientServer clientServer;
    // 开启线程，用于处理从服务端发送过来的数据
    startServerDataProcess(&clientServer);

    g_chatBox = new ChatBox(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    bool running = true;
    SDL_Event event;

    while (running) {

        PlayerInfo playerInfo = g_player->generatePlayerInfo();


        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                clientServer.send_message(ENUM_MSG_DELETE_PLAYER_REQUEST, g_player->generatePlayerJson(playerInfo));
                // 程序退出时，发送退出消息
                spdlog::info("游戏退出！");
                // 暂停一秒，等待数据发送出去再退出
                // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                
            }
            else if (event.type == SDL_WINDOWEVENT){
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    g_player->setCameraRect(event.window.data1, event.window.data2);
                }
            }
            g_chatBox->handleEvent(&event);

        }

        // 处理输入
        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        g_player->handleInput(keyState);

        // 渲染
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        Camera camera = g_player->getCamera();
        SDL_Rect bg_src_rect = {
            camera.x,                    // 裁剪背景的起始 X
            camera.y,                    // 裁剪背景的起始 Y
            camera.width,                // 裁剪宽度 = 摄像机宽度
            camera.height                // 裁剪高度 = 摄像机高度
        };

        SDL_Rect bg_dest_rect = {
            0,                          // 渲染到屏幕左上角
            0,
            camera.width,               // 渲染宽度 = 窗口宽度
            camera.height               // 渲染高度 = 窗口高度
        };

        // 渲染背景
        SDL_RenderCopy(renderer, bgTexture, &bg_src_rect, &bg_dest_rect);
        // 将player进行渲染
        g_player->render(renderer);
        g_chatBox->render();

        clientServer.send_message(ENUM_MSG_REGISTER_UPDATE_PLAYER_REQUEST, g_player->generatePlayerJson(playerInfo));
        clientServer.send_message(ENUM_MSG_SENDMESSAGE_REQUEST, g_chatBox->generateSendMsg());

        SDL_RenderPresent(renderer);

        SDL_Delay(10); // 控制动画帧速率
    }
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
