#include "interfaceManager.hpp"
#include "clientComonFunc.hpp"



InterfaceManager::InterfaceManager()
{
}

InterfaceManager::~InterfaceManager()
{
}

InterfaceManager &InterfaceManager::getInstance()
{
    static InterfaceManager instance;
    return instance;
}

bool InterfaceManager::initWindow()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        spdlog::error("SDL初始化失败: " + std::string(SDL_GetError()));
        return false;
    }
    
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        spdlog::error("SDL_image初始化失败: " + std::string(IMG_GetError()));
        SDL_Quit();
        return false;
    }

    window = SDL_CreateWindow(GAME_NAME,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) {
        spdlog::error( "无法创建窗口: " + std::string(SDL_GetError()) );
        IMG_Quit();
        SDL_Quit();
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        spdlog::error( "无法创建渲染器: " + std::string(SDL_GetError()) );
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    return true;
}

void InterfaceManager::closeWindow()
{
        // 清理资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

SDL_Renderer *InterfaceManager::renderer() const
{
    return renderer;
}
