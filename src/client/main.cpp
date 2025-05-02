#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.hpp"
#include "./HttpServer/clientServer.hpp"
#include "clientComonFunc.hpp"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;


void startServerDataProcess(ClientServer* clientServer){
    std::thread t = std::thread(&ClientServer::dealServerData,  clientServer);
    t.detach();
}


int main() {  // int argc, char* argv[]

    ClientComonFunc::getInstance();

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("SDL2 Player Animation",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    g_player = new Player(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 5, renderer);

    ClientServer clientServer;
    // 开启线程，用于处理从服务端发送过来的数据
    startServerDataProcess(&clientServer);

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // 处理输入
        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        g_player->handleInput(keyState);

        // 渲染
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        // 将player进行渲染
        g_player->render(renderer);

        PlayerInfo playerInfo = g_player->generatePlayerInfo();

        clientServer.send_message(ENUM_MSG_REGISTER_UPDATE_PLAYER_REQUEST, g_player->generatePlayerJson(playerInfo));

        SDL_RenderPresent(renderer);

        SDL_Delay(10); // 控制动画帧速率
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
