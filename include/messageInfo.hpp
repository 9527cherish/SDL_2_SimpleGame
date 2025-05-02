#pragma once 

#include <string>



enum ENUM_MSG_TYPE
{
    ENUM_MSG_REGISTER_UPDATE_PLAYER_REQUEST = 0,  // 人物注册及更新
    ENUM_MSG_REGISTER_UPDATE_PLAYER_RESPONSE = 1,    // 登录回复
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

