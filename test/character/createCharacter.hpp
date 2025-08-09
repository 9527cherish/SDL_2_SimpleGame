#pragma once

#include <filesystem>
#include <pugixml.hpp>
#include <vector>
#include <string>
#include <map>
#include <spdlog/spdlog.h>

// 动画方向枚举
enum class Direction{
    DOWN,
    LEFT,
    UP,
    RIGHT,
    DEFAULT,
    UNKNOWN
};

// 定义动作
enum class CharacterAction{
    DEFAULT,
    STAND,
    WALK,
    SIT,
    DEAD,
    ATTACK,
    ATTACK_SWORD_STAB,
    ATTACK_BOW,
    ATTACK_SPEAR,
    ATTACK_CHOP,
    ATTACK_2HAND,
    CAST
};

// 动画帧结构
struct Frame {
    int index = 0; // 帧索引
    int offsetX = 0;    // X轴偏移
    int offsetY = 0;    // Y轴偏移
    int delay = 0; // 延迟时间（毫秒）
};

// 动画序列结构
struct AnimationSequence {
    std::vector<Frame> frames;
    bool loop = true; // 是否循环
};

// 方向动画映射
struct DirectionAnimation {
    std::map<Direction, AnimationSequence> animations;
};

// 动作结构
struct Action {
    std::string name;
    std::string imageset;
    DirectionAnimation directionAnim;
};

// Sprite 数据结构  ,重新设计，不太好遍历查找
struct SpriteData {
    int variants = 1;
    int variantOffset = 0;
    std::string imageSetName;
    std::string imageSetSrc;
    int frameWidth = 0;
    int frameHeight = 0;
    std::vector<Action> actions;
};

// 颜色组结构
struct ColorGroup {
    std::vector<std::string> colors;
};

// 精灵部件结构
struct SpritePart {
    std::string path;               // 精灵文件路径
    std::vector<ColorGroup> groups; // 颜色组列表
};

// NPC 结构
struct NPC {
    int id;                         // NPC ID
    std::vector<SpritePart> parts;  // 精灵部件列表
};

struct ImageSet
{
    int variants = 1;
    int variantOffset = 0;
    std::string imageSetName;
    std::string imageSetPath;
    std::vector<std::string> imageSetParam;
    int frameWidth = 0;
    int frameHeight = 0;
    std::string file;
};

inline AnimationSequence getFrameFromSprite(const SpriteData& sprite
            , const std::string actionName, const Direction& direction)
{
    if(sprite.actions.size() < 1)
        return AnimationSequence();

    for(Action action : sprite.actions)
    {
        if(action.name == actionName)
        {
            auto iter = action.directionAnim.animations.find(direction);
            if(iter != action.directionAnim.animations.end())
            {
                return iter->second;
            }

        }
    }
    return AnimationSequence();
}

// // 颜色结构体
// struct Color {
//     Uint8 r, g, b, a;
    
//     Color() : r(0), g(0), b(0), a(255) {}
//     Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) : r(r), g(g), b(b), a(a) {}
    
//     bool operator==(const Color& other) const {
//         return r == other.r && g == other.g && b == other.b && a == other.a;
//     }
// };

class CreateCharacter
{
public:
    CreateCharacter();
    ~CreateCharacter();
    void init_logger(const std::string& log_name);

    void parseAllNpcXml();
    NPC parseSingleNpcXml();

    std::vector<ColorGroup> parseColorGroups(const std::string& colorStr);
    NPC parseNpcXml(const std::string& path);
    void parseNpcXml();
    void printNPCData(const NPC& npc);
    void printNPCData();

    void parseNeutralXml();
    void parseHairstyleXml();

    ImageSet parseImageXml(const std::string& path);
    void printImageXml(const ImageSet& imageSet);

    SpriteData parseSpriteXML(const std::string& path);
    void printSpriteData(const SpriteData& spriteData);

    std::string getXmlPath();
    std::string getGraphicsPath();

    // Color parseHexColor(const std::string& hex);

    Frame getCurrentFrame();
private:

    std::string logPath;
    std::string logName;

    std::string xmlPath;
    std::string graphicsPath;
    std::string npcXmlPath;
    std::string neutralXmlPath;
    std::string hairstyleXmlPath;

    std::vector<std::string> vecNpcXmlpath;

    uint iFrame;

};
