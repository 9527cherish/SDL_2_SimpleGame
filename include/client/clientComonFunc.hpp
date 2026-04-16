#pragma once
#include <string>
#include <spdlog/spdlog.h>
#include "json.hpp"
using json = nlohmann::json;

// ClientComonFunc 负责客户端日志初始化、崩溃处理和公共工具能力
class ClientComonFunc{
public:

    ~ClientComonFunc();

    // 返回客户端公共工具单例。
    static ClientComonFunc& getInstance();
    // 弹出崩溃提示对话框。
    static void showCrashDialog(const char* title, const char* message);
    // 安装进程崩溃信号与终止处理器。
    static void installCrashHandlers();
    // 初始化客户端日志系统。
    void init_logger(const std::string& log_name);

    // 获取当前时间字符串。
    std::string getCurrentTime();


private:

    // 日志目录路径
    std::string logPath;
    // 当前日志文件名
    std::string logName;

    
    ClientComonFunc();
    // 禁用拷贝构造
    ClientComonFunc(ClientComonFunc&) = delete;
    ClientComonFunc(ClientComonFunc&&) = delete;

    // 禁用赋值
    ClientComonFunc& operator=(const ClientComonFunc&) = delete;
    ClientComonFunc& operator=(const ClientComonFunc&&) = delete;

};
