#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "clientComonFunc.hpp"
#include "characterStruct.hpp"
#include "dataManager.hpp"

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
    // SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    DataManager::getInstance().initData();

    
    return 0;
}
