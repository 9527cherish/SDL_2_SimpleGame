#include "characterDirection.hpp"

std::string DirectionMapper::to_string(CharaDirection direction)
{
    auto it = m_enumToStr.find(direction);
    if (it == m_enumToStr.end()) {
        spdlog::error("无效的方向枚举值");
    }
    return it->second;
}

CharaDirection DirectionMapper::from_string(const std::string &str)
{
    auto it = m_strToEnum.find(str);
    if (it == m_strToEnum.end()) {
        spdlog::error("无效的动作字符串: " + str);
    }
    return it->second;
}

// 4. 自动初始化映射表
#define X(enum_name, str) {CharaDirection::enum_name, str},
const std::unordered_map<CharaDirection, std::string> DirectionMapper::m_enumToStr = {
    DIRECTION_LIST
};
#undef X

#define X(enum_name, str) {str, CharaDirection::enum_name},
const std::unordered_map<std::string, CharaDirection> DirectionMapper::m_strToEnum = {
    DIRECTION_LIST
};
#undef X
