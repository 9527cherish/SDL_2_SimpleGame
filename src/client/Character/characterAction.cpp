#include "characterAction.hpp"

// 4. 自动初始化映射表
#define X(enum_name, str) {CharaAction::enum_name, str},
const std::unordered_map<CharaAction, std::string> ActionMapper::m_enumToStr = {
    ACTION_LIST
};
#undef X

#define X(enum_name, str) {str, CharaAction::enum_name},
const std::unordered_map<std::string, CharaAction> ActionMapper::m_strToEnum = {
    ACTION_LIST
};
#undef X


std::string ActionMapper::to_string(CharaAction action)
{
    auto it = m_enumToStr.find(action);
    if (it == m_enumToStr.end()) {
        spdlog::error("无效的动作枚举值");
    }
    return it->second;
}

CharaAction ActionMapper::from_string(const std::string &str)
{
    auto it = m_strToEnum.find(str);
    if (it == m_strToEnum.end()) {
        spdlog::error("无效的动作字符串: " + str);
    }
    return it->second;
}
