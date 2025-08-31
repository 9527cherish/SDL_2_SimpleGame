
#pragma once 


const int SCREEN_WIDTH = 800;  // 屏幕大小
const int SCREEN_HEIGHT = 600;
const int MAP_WIDTH = 1024;     // 地图大小
const int MAP_HEIGHT = 1024;

const int PLAYER_WIDTH = 64;     // 玩家大小
const int PLAYER_HEIGHT = 64;


enum class Scene {
    MAIN_MENU,    // 主菜单场景
    GAME_SCENE,   // 游戏场景
    SETTINGS      // 设置场景（预留）
};