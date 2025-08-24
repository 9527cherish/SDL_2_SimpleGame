
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
    // 自动生成 枚举->字符串 映射表
    static const std::unordered_map<CharaDirection, std::string> m_enumToStr;
    // 自动生成 字符串->枚举 映射表
    static const std::unordered_map<std::string, CharaDirection> m_strToEnum;

public:
    static std::string to_string(CharaDirection direction);

    static CharaDirection from_string(const std::string& str);

};
