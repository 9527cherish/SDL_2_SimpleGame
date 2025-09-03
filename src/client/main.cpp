
#include "clientComonFunc.hpp"
#include "characterStruct.hpp"
#include "dataManager.hpp"
#include "interfaceManager.hpp"
#include "gameInterface.hpp"

int main(int argc, char* argv[]) {

    ClientComonFunc::getInstance();
    // DataManager::getInstance().initData();
    InterfaceManager::getInstance().initWindow();

    
    GameInterface* gameScene = nullptr;
    bool quit = false;
    SDL_Event e;

     while (!quit) {
        // 处理事件
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            SDL_Delay(30); // 约60FPS
        }
     }


    InterfaceManager::getInstance().closeWindow();
    
    return 0;
}
