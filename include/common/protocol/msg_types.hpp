#pragma once

enum ENUM_MSG_TYPE
{
    ENUM_MSG_REGISTER_UPDATE_PLAYER_REQUEST = 0,  // 人物注册及更新
    ENUM_MSG_REGISTER_UPDATE_PLAYER_RESPONSE = 1,    // 人物广播回复

    ENUM_MSG_DELETE_PLAYER_REQUEST,    // 删除人物请求（某个客户端异常下线的场景）
    ENUM_MSG_DELETE_PLAYER_RESPONSE,   // 删除人物回复

    ENUM_MSG_SYNC_PLAYERS_REQUEST,     // 同步当前在线人物列表
    ENUM_MSG_SYNC_PLAYERS_RESPONSE,    // 当前在线人物列表回复

    ENUM_MSG_SYNC_TREES_REQUEST,       // 同步树木列表
    ENUM_MSG_SYNC_TREES_RESPONSE,      // 树木列表回复

    ENUM_MSG_HIT_TREE_REQUEST,         // 砍树请求
    ENUM_MSG_HIT_TREE_RESPONSE,        // 砍树结果

    ENUM_MSG_UPDATE_TREE_PUSH,         // 树木状态推送

    ENUM_MSG_SENDMESSAGE_REQUEST,  // 发送消息请求
    ENUM_MSG_SENDMESSAGE_RESPONSE,    // 发送消息回复
};

enum ENUM_TREE_STATE
{
    ENUM_TREE_STATE_ALIVE = 0,
    ENUM_TREE_STATE_STUMP = 1
};
