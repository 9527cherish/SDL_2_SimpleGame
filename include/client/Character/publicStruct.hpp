#pragma once 

#include <vector>
#include <map>
#include "characterDirection.hpp"
#include "characterAction.hpp"


const int SCREEN_WIDTH = 800;  // 屏幕大小
const int SCREEN_HEIGHT = 600;
const int MAP_WIDTH = 1024;     // 地图大小
const int MAP_HEIGHT = 1024;

const int PLAYER_WIDTH = 64;     // 玩家大小
const int PLAYER_HEIGHT = 64;

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

// 动作结构(包括方向)
struct Action {
    CharaAction action;
    std::string imageset;
    CharaDirection direction;

    Action()
    {
        action = CharaAction::DEFAULT;
        direction = CharaDirection::DEFAULT;
    }

    Action(const CharaAction& action_, const CharaDirection& direction_)
    {
        action = action_;
        direction = direction_;
    }

    bool operator==(const Action& other) const {
        return action == other.action && direction == other.direction;
    }

    bool operator!=(const Action& other) const {
        return action != other.action ||  direction != other.direction;
    }

        // 重载 < 运算符：定义两个 Action 如何比较
    bool operator<(const Action& other) const {
        if (action != other.action) {
            return action < other.action;
        }
        // 2. id 相同则比较 name（字符串可直接用 < 比较）
        return action < other.action;
    }
};

// Sprite 结构
struct SpriteData {
    int variants = 1;
    int variantOffset = 0;
    std::string imageSetName;
    std::string imageSetSrc;
    int frameWidth = 0;
    int frameHeight = 0;
    std::map<Action, AnimationSequence> animations;

    SpriteData()
    {
        imageSetName = imageSetSrc = "";
    }
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
    std::string name;               // 存储名字
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

    ImageSet()
    {
        imageSetName = imageSetPath = file = "";
    }
};

// inline AnimationSequence getFrameFromSprite(const SpriteData& sprite
//             , const std::string actionName, const Direction& direction)
// {
//     if(sprite.actions.size() < 1)
//         return AnimationSequence();

//     for(Action action : sprite.actions)
//     {
//         if(action.name == actionName)
//         {
//             auto iter = action.directionAnim.animations.find(direction);
//             if(iter != action.directionAnim.animations.end())
//             {
//                 return iter->second;
//             }

//         }
//     }
//     return AnimationSequence();
// }
