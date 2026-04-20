#pragma once

#include <memory>
#include <string>
#include "persona.hpp"

// PlayerSession 保存本机当前选择的人物实例和索引
class PlayerSession
{
public:
    // 只保存当前本机玩家会话，不负责资源加载与远端同步。
    static PlayerSession& getInstance();

    // 直接设置当前本机人物实例。
    void setCurrentPersona(std::shared_ptr<Persona> persona);
    // 按资源索引选择并创建当前人物实例。
    void selectPersona(int index);
    // 返回当前本机人物实例。
    std::shared_ptr<Persona> currentPersona();
    // 返回当前本机人物索引。
    int currentPersonaIndex() const;
    // 设置本机玩家 UUID。
    void setPlayerUuid(const std::string& uuid);
    // 返回本机玩家 UUID。
    const std::string& playerUuid() const;

private:
    PlayerSession() = default;
    PlayerSession(const PlayerSession&) = delete;
    PlayerSession& operator=(const PlayerSession&) = delete;

private:
    // 当前本机使用的人物实例
    std::shared_ptr<Persona> m_currentPersona;
    // 当前本机选择的人物模板索引
    int m_currentPersonaIndex = -1;
    // 当前本机玩家 UUID
    std::string m_playerUuid;
};
