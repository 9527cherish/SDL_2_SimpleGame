
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
    // 自动生成 枚举->字符串 映射表
    static const std::unordered_map<CharaAction, std::string> m_enumToStr;
    // 自动生成 字符串->枚举 映射表
    static const std::unordered_map<std::string, CharaAction> m_strToEnum;

public:
    static std::string to_string(CharaAction action);

    static CharaAction from_string(const std::string& str);
};


