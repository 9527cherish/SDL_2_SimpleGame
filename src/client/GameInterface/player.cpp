#include "player.hpp"
#include <iostream>
#include <filesystem>
#include <pugixml.hpp>
#include <uuid/uuid.h>
#include "clientComonFunc.hpp"
#include <spdlog/spdlog.h>
#include "messageInfo.hpp"


Player* g_player = nullptr;

Player::Player(int startX, int startY, int moveSpeed, SDL_Renderer* renderer) {

    m_xmlPath = "../images/character_malePerson_sheetHD.xml";
    initTextures(renderer);

    x = startX;
    y = startY;
    speed = moveSpeed;
    frame = 0;
    m_uuid = generateUuid();
    pictureName = "walk0";
    m_name = "";

}

Player::~Player() {
    SDL_DestroyTexture(m_atlasTexture);
}

void Player::handleInput(const Uint8* keyState) {

    bMove.store(false);
    if (keyState[SDL_SCANCODE_W]) { 
        y -= speed; 
        y = (y < 0) ? 0 : ((y > MAP_HEIGHT) ? MAP_HEIGHT : y);
        bMove.store(true); 
    }
    if (keyState[SDL_SCANCODE_S]) { 
        y += speed; 
        y = (y < 0) ? 0 : ((y > MAP_HEIGHT) ? MAP_HEIGHT : y);
        bMove.store(true); 
    }
    if (keyState[SDL_SCANCODE_A]) { 
        x -= speed; 
        x = (x < 0) ? 0 : ((x > MAP_WIDTH) ? MAP_WIDTH : x);
        bMove.store(true); 
    }
    if (keyState[SDL_SCANCODE_D]) { 
        x += speed; 
        x = (x < 0) ? 0 : ((x > MAP_WIDTH) ? MAP_WIDTH : x);
        bMove.store(true);
    }

    // 切换动画帧
    if (bMove.load()) frame = (frame + 1) % 8;


    // 更新相机位置
    m_camera.CenterOn(x, y);

    // 处理地图边界（防止摄像机越界）
    if (m_camera.x < 0) m_camera.x = 0;
    if (m_camera.y < 0) m_camera.y = 0;
    if (m_camera.x > MAP_WIDTH - m_camera.width) m_camera.x = MAP_WIDTH - m_camera.width;
    if (m_camera.y > MAP_HEIGHT - m_camera.height) m_camera.y = MAP_HEIGHT - m_camera.height;
}

void Player::render(SDL_Renderer* renderer) {

    renderPlayer(renderer);
    renderEnemy(renderer);
}

void Player::renderEnemy(SDL_Renderer *renderer)
{

    std::lock_guard<std::mutex> lock(m_csEnemy);
    for(auto iter : m_enemyMap)
    {
        PlayerInfo enemy = iter.second;
        SDL_Rect dstRect = { enemy.x, enemy.y, PLAYER_WIDTH, PLAYER_HEIGHT }; // 渲染大小
        SDL_Rect srcRect =  { enemy.texture.x, enemy.texture.y, 
                                enemy.texture.width, enemy.texture.height };
        
        SDL_RenderCopy(renderer, m_atlasTexture, &srcRect, &dstRect);
    }
}

void Player::renderPlayer(SDL_Renderer *renderer)
{
    SDL_Rect srcRect = std::get<1>(m_classifyRect["walk"][frame]);
    pictureName = std::get<0>(m_classifyRect["walk"][frame]);
    SDL_Rect dstRect = {
        (x - m_camera.x) - PLAYER_WIDTH / 2,
        (y - m_camera.y) - PLAYER_HEIGHT / 2,
        PLAYER_WIDTH,
        PLAYER_HEIGHT
    };
    SDL_RenderCopy(renderer, m_atlasTexture, &srcRect, &dstRect);
}


bool Player::loadFromFile(const std::string &xmlPath)
{
    pugi::xml_document doc;
        if (!doc.load_file(xmlPath.c_str())) return false;

        pugi::xml_node atlas = doc.child("TextureAtlas");
        m_imagePath = atlas.attribute("imagePath").as_string();

        for (pugi::xml_node node : atlas.children("SubTexture")) {
            SubTexture st;
            st.name = node.attribute("name").as_string(),
            st.x = node.attribute("x").as_int(),
            st.y = node.attribute("y").as_int(),
            st.width = node.attribute("width").as_int(),
            st.height = node.attribute("height").as_int();
            m_subTextures[st.name] = st;
        }

        return true;
}

const SubTexture *Player::getSubTexture(const std::string &name) const
{
    auto it = m_subTextures.find(name);
        return (it != m_subTextures.end()) ? &it->second : nullptr;
}

const SDL_Rect Player::getRect(const std::string &name) const
{
    const SubTexture* st = getSubTexture(name);
    if(nullptr != st){
        SDL_Rect srcRect { st->x, st->y, st->width, st->height };
        return srcRect;
    }
    else{
        SDL_Rect srcRect { 0,0,0,0};
        spdlog::error("该纹理没有对应的 SDL_Rect:" + name);
        return srcRect;
    }

}

void Player::initTextures(SDL_Renderer* renderer)
{
    if (!loadFromFile(m_xmlPath)) {
        spdlog::error("加载图片 XML失败:" + m_xmlPath);
    }
    else{
        spdlog::info("加载图片 XML成功:" + m_xmlPath);
    }

    m_atlasTexture = IMG_LoadTexture(renderer, m_imagePath.c_str());

    if(!m_atlasTexture){
        spdlog::error("使用IMG_LoadTexture将png转化为Texture失败");
        
    }
    else{
        spdlog::info("使用IMG_LoadTexture将png转化为Texture成功");
    }

    // std::vector<SDL_Rect>vecRect;
    // vecRect.emplace_back(getRect("walk0"));
    m_classifyRect["walk"].emplace_back("walk0", getRect("walk0"));
    m_classifyRect["walk"].emplace_back("walk1", getRect("walk1"));
    m_classifyRect["walk"].emplace_back("walk2", getRect("walk2"));
    m_classifyRect["walk"].emplace_back("walk3", getRect("walk3"));
    m_classifyRect["walk"].emplace_back("walk4", getRect("walk4"));
    m_classifyRect["walk"].emplace_back("walk5", getRect("walk5"));
    m_classifyRect["walk"].emplace_back("walk6", getRect("walk6"));
    m_classifyRect["walk"].emplace_back("walk7", getRect("walk7"));
    
    m_classifyRect["run"].emplace_back("run0", getRect("run0"));
    m_classifyRect["run"].emplace_back("run1", getRect("run1"));
    m_classifyRect["run"].emplace_back("run2", getRect("run2")); 
}

std::string Player::generateUuid()
{
    uuid_t uuid;
    uuid_generate(uuid);

    char str[37];
    uuid_unparse(uuid, str);
    return std::string(str);
}

std::string Player::getUuid()
{
    return m_uuid;
}

PlayerInfo Player::generatePlayerInfo()
{
    PlayerInfo playerInfo;
    playerInfo.uuid = m_uuid;
    playerInfo.name = "";  // 后续提供
    playerInfo.x = x;
    playerInfo.y = y;
    playerInfo.texture = *(getSubTexture(pictureName));

    return playerInfo;
}

std::string Player::generatePlayerJson(const PlayerInfo playerInfo)
{
    json playerJson;
    playerJson["uuid"] = playerInfo.uuid;
    playerJson["name"] = playerInfo.name;
    playerJson["x"] = playerInfo.x;
    playerJson["y"] = playerInfo.y;
    
    json textureJson;
    textureJson["name"] = playerInfo.texture.name;
    textureJson["x"] = playerInfo.texture.x;
    textureJson["y"] = playerInfo.texture.y;
    textureJson["width"] = playerInfo.texture.width;
    textureJson["height"] = playerInfo.texture.height;

    playerJson["texture"] = textureJson;

    return playerJson.dump();
}

PlayerInfo Player::generatePlayerInfo(const json& js)
{
    PlayerInfo playerInfo;
    playerInfo.uuid = js["uuid"];
    playerInfo.name = js["name"];
    playerInfo.x = js["x"];
    playerInfo.y = js["y"];
    
    playerInfo.texture.name = js["texture"]["name"] ;
    playerInfo.texture.x = js["texture"]["x"];
    playerInfo.texture.y = js["texture"]["y"];
    playerInfo.texture.width = js["texture"]["width"];
    playerInfo.texture.height = js["texture"]["height"];
    
    return playerInfo;
}

void Player::addAndUpgradeEnemy(const PlayerInfo &enemy)
{
    std::lock_guard<std::mutex> lock(m_csEnemy);
    m_enemyMap[enemy.uuid] = enemy;
}

bool Player::deleteEnemy(const PlayerInfo &enemy)
{
    std::lock_guard<std::mutex> lock(m_csEnemy);
    auto iter = m_enemyMap.find(enemy.uuid);
    if(iter != m_enemyMap.end())
    {
        m_enemyMap.erase(iter);
        return true;
    }
    return false;

}

bool Player::isMove()
{
    return bMove.load();
}

Camera Player::getCamera()
{
    return m_camera;
}

void Player::setCameraPos(int x, int y)
{
    m_camera.x = x;
    m_camera.y = y;
}

void Player::setCameraRect(int width, int height)
{
    m_camera.width = width;
    m_camera.height = height;
}

std::string Player::getName()
{
    return m_name;
}
