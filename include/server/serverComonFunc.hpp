#pragma once

#include <string>

// ServerComonFunc 负责服务端日志初始化等公共能力
class ServerComonFunc
{

public:
    ServerComonFunc();
    ~ServerComonFunc();

    // 返回服务端公共工具单例。
    static ServerComonFunc& getInstance();
    
    // 初始化服务端日志系统。
    void init_logger(const std::string& log_name);

private:

    // 日志目录路径
    std::string logPath;
    // 当前日志文件名
    std::string logName;
};
