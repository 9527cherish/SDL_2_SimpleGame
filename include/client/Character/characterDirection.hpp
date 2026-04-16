
#pragma once 
#include <string>
#include <unordered_map>
#include "clientComonFunc.hpp"

// 1. 核心：用X宏定义所有动作（名称+字符串）
#define DIRECTION_LIST \
    X(UNKNOWN, "unknown")   \
    X(DEFAULT, "default")     \
    X(DOWN, "down")   \
    X(LEFT, "left") \
    X(UP, "up")   \
    X(RIGHT, "right")

// 2. 自动生成枚举
enum class CharaDirection {
#define X(enum_name, str) enum_name,
    DIRECTION_LIST
#undef X
};

// 3. 自动生成转换工具类
class DirectionMapper {
private:
    static const std::unordered_map<CharaDirection, std::string> m_enumToStr; // 朝向枚举到字符串的映射表。
    static const std::unordered_map<std::string, CharaDirection> m_strToEnum; // 朝向字符串到枚举的映射表。

public:
    // 把方向枚举转换为字符串表示。
    static std::string to_string(CharaDirection direction);

    // 把方向字符串解析为内部方向枚举。
    static CharaDirection from_string(const std::string& str);

};
