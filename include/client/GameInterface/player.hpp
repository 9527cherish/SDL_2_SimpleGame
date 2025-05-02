#ifndef PLAYER_H
#define PLAYER_H
    
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <mutex>
#include <atomic>
#include "enemy.hpp"
#include "messageInfo.hpp"
#include "json.hpp"
using json = nlohmann::json;

class Player {
public:
    int x, y;
    int speed;
    int frame; // 当前帧

    Player(int startX, int startY, int moveSpeed, SDL_Renderer* renderer);
    ~Player();
    void handleInput(const Uint8* keyState);
    void render(SDL_Renderer* renderer);


    // 渲染敌人
    void renderEnemy(SDL_Renderer* renderer);
    // 渲染晚间
    void renderPlayer(SDL_Renderer* renderer);
    // 加载xml
    bool loadFromFile(const std::string& xmlPath);
    // 加载xml中的动作坐标信息
    const SubTexture* getSubTexture(const std::string& name) const;
    // 获取动作坐标信对应的SDL_Rect
    const SDL_Rect getRect(const std::string& name) const;

    void initTextures(SDL_Renderer* renderer);

    // 生成uuid
    std::string generateUuid(); 
    std::string getUuid();

    // 生成palyerInfo信息
    PlayerInfo generatePlayerInfo();
    // 将playerInfo转化为json
    std::string generatePlayerJson(const PlayerInfo playerInfo);
    
    PlayerInfo generatePlayerInfo(const json& js);

    // 增加、更新或删除敌人
    void addAndUpgradeEnemy(const PlayerInfo& enemy);

    bool deleteEnemy(const PlayerInfo& enemy);

    bool isMove();

private:
    // XML文件路径
    std::string m_xmlPath;
    // 图片路径
    std::string m_imagePath;
    // xml中所有动作相对于图片坐标信息
    std::unordered_map<std::string, SubTexture> m_subTextures;
    // 整个png 对应的纹理 
    SDL_Texture* m_atlasTexture;
    // 按照动作将图片进行分类
    std::unordered_map<std::string, std::vector<std::tuple<std::string, SDL_Rect>>> m_classifyRect;
    
    // 所有敌人
    std::mutex m_csEnemy;
    std::unordered_map<std::string, PlayerInfo> m_enemyMap;
    // 唯一标识uuid
    std::string m_uuid;
    // 当前所使用的图片名称
    std::string pictureName;
    // 标识当前是否在移动
    std::atomic<bool> bMove{false};
};

extern  Player* g_player;
#endif
