#pragma once 

#include <vector>
#include <map>
#include "characterDirection.hpp"
#include "characterAction.hpp"


// 动画帧结构
struct Frame {
    int index = 0; // 帧索引
    int offsetX = 0;    // X轴偏移
    int offsetY = 0;    // Y轴偏移
    int delay = 0; // 延迟时间（毫秒）
};

// 动画序列结构
struct AnimationSequence {
    std::vector<Frame> frames; // 当前动作序列包含的所有帧。
    bool loop = true; // 当前动作序列是否循环播放。
};

// 动作结构(包括方向)
struct Action {
    CharaAction action;            // 当前动作类型。
    std::string imageset;          // 动作所引用的图片集名称。
    CharaDirection direction;      // 当前动作对应的方向。

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

    // 判断两个动作键是否完全相同。
    bool operator==(const Action& other) const {
        return action == other.action && direction == other.direction;
    }

    // 判断两个动作键是否不同。
    bool operator!=(const Action& other) const {
        return action != other.action ||  direction != other.direction;
    }

    // 为 std::map 定义动作键的排序规则。
    bool operator<(const Action& other) const {
        if (action != other.action) {
            return action < other.action;
        }
        return direction < other.direction;
    }
};

// Sprite 结构
struct SpriteData {
    int variants = 1;                                // 当前精灵支持的变体数量。
    int variantOffset = 0;                           // 变体在图集中的起始偏移。
    std::string imageSetName;                        // 引用的图片集名称。
    std::string imageSetSrc;                         // 引用的图片集路径。
    int frameWidth = 0;                              // 单帧宽度。
    int frameHeight = 0;                             // 单帧高度。
    std::map<Action, AnimationSequence> animations; // 动作与动画序列的映射表。

    SpriteData()
    {
        imageSetName = imageSetSrc = "";
    }
};


// 颜色组结构
struct ColorGroup {
    std::vector<std::string> colors; // 同一颜色组下可选的颜色列表。
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
    int variants = 1;                          // 图片集支持的变体数量。
    int variantOffset = 0;                     // 图片集变体的起始偏移。
    std::string imageSetName;                  // 图片集名称。
    std::string imageSetPath;                  // 图片集文件路径。
    std::vector<std::string> imageSetParam;    // 图片集的额外参数。
    int frameWidth = 0;                        // 图片集单帧宽度。
    int frameHeight = 0;                       // 图片集单帧高度。
    std::string file;                          // 原始文件名。

    ImageSet()
    {
        imageSetName = imageSetPath = file = "";
    }
};
