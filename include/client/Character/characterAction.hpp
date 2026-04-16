
#pragma once 
#include <string>
#include <unordered_map>
#include "clientComonFunc.hpp"

// 1. 核心：用X宏定义所有动作（名称+字符串）
#define ACTION_LIST \
    X(UNKNOWN, "unknown")   \
    X(DEFAULT, "default")     \
    X(STAND, "stand")   \
    X(WALK, "walk") \
    X(SIT, "sit")   \
    X(DEAD, "dead")     \
    X(ATTACK, "attack")   \
    X(ATTACK_SWORD_STAB, "attack_sword_stab") \
    X(ATTACK_BOW, "attack_bow")     \
    X(ATTACK_SPEAR, "attack_spear")   \
    X(ATTACK_CHOP, "attack_chop") \
    X(ATTACK_2HAND, "attack_2hand")   \
    X(CAST, "cast")



// 2. 自动生成枚举
enum class CharaAction {
#define X(enum_name, str) enum_name,
    ACTION_LIST
#undef X
};

// 3. 自动生成转换工具类
class ActionMapper {
private:
    static const std::unordered_map<CharaAction, std::string> m_enumToStr; // 动作枚举到字符串的映射表。
    static const std::unordered_map<std::string, CharaAction> m_strToEnum; // 动作字符串到枚举的映射表。

public:
    // 把动作枚举转换为网络或配置可用的字符串。
    static std::string to_string(CharaAction action);

    // 把动作字符串解析为内部动作枚举。
    static CharaAction from_string(const std::string& str);
};

