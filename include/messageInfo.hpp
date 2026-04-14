#pragma once 

#include <string>

const int SCREEN_WIDTH = 800;  // 屏幕大小
const int SCREEN_HEIGHT = 600;
const int MAP_WIDTH = 1024;     // 地图大小
const int MAP_HEIGHT = 1024;

const int PLAYER_WIDTH = 64;     // 玩家大小
const int PLAYER_HEIGHT = 64;


enum ENUM_MSG_TYPE
{
    ENUM_MSG_REGISTER_UPDATE_PLAYER_REQUEST = 0,  // 人物注册及更新
    ENUM_MSG_REGISTER_UPDATE_PLAYER_RESPONSE = 1,    // 登录回复

    ENUM_MSG_DELETE_PLAYER_REQUEST,    // 删除人物请求（某个客户端异常下线的场景）
    ENUM_MSG_DELETE_PLAYER_RESPONSE,   // 删除人物回复

    ENUM_MSG_SENDMESSAGE_REQUEST,  // 发送消息请求
    ENUM_MSG_SENDMESSAGE_RESPONSE,    // 发送消息回复
};



struct SubTexture {
    std::string name;
    int x;
    int y;
    int width;
    int height;
    SubTexture(){
        name = "";
        x = y = width = height = 0;
    }
};

struct PlayerInfo
{
    std::string uuid;         // 唯一标识
    std::string name;         // 角色名称  
    int x;                    // 位置 x
    int y;                    // 位置 y
    SubTexture texture;      // 采用的纹理

    PlayerInfo(){
        uuid = name = "";
        x = y = 0;
    }
};

