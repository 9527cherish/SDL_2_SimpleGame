#pragma once
#include <string>
#include <spdlog/spdlog.h>
#include "json.hpp"
using json = nlohmann::json;


class ClientComonFunc{
public:

    ~ClientComonFunc();

    static ClientComonFunc& getInstance();
    // 初始化日志
    void init_logger(const std::string& log_name);

    // 增加时间戳
    std::string getCurrentTime();


private:

    std::string logPath;
    std::string logName;

    
    ClientComonFunc();
    // 禁用拷贝构造
    ClientComonFunc(ClientComonFunc&) = delete;
    ClientComonFunc(ClientComonFunc&&) = delete;

    // 禁用赋值
    ClientComonFunc& operator=(const ClientComonFunc&) = delete;
    ClientComonFunc& operator=(const ClientComonFunc&&) = delete;

};