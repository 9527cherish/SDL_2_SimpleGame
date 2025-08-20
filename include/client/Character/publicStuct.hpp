#pragma once 

#include <string>
#include <vector>
#include <map>

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
