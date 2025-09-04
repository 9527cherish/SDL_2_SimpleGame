
#pragma once 


#include <vector>
#include <map>
#include <string>
#include <SDL2/SDL.h>


#define GAME_NAME "SDL_2 rogue"


const int SCREEN_WIDTH = 1000;  // 屏幕大小
const int SCREEN_HEIGHT = 800;
const int MAP_WIDTH = 1024;     // 地图大小
const int MAP_HEIGHT = 1024;

const int PLAYER_WIDTH = 64;     // 玩家大小
const int PLAYER_HEIGHT = 64;


enum class Scene {
    MAIN_MENU,    // 主菜单场景
    GAME_SCENE,   // 游戏场景
    SETTINGS      // 设置场景（预留）
};


// // 颜色定义
// struct Color {
//     Uint8 r, g, b, a;
//     Color(Uint8 r=0, Uint8 g=0, Uint8 b=0, Uint8 a=255) : r(r), g(g), b(b), a(a) {}

//         // 添加转换为 SDL_Color 的方法
//     operator SDL_Color() const {
//         SDL_Color sdlColor;
//         sdlColor.r = r;
//         sdlColor.g = g;
//         sdlColor.b = b;
//         sdlColor.a = a;
//         return sdlColor;
//     }
// };