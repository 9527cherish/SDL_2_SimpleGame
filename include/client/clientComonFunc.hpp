#pragma once
#include <string>
#include <spdlog/spdlog.h>
#include "json.hpp"
using json = nlohmann::json;



class ClientComonFunc{
public:
    ClientComonFunc();
    ~ClientComonFunc();

    static ClientComonFunc& getInstance();
    // 出事化日志
    void init_logger(const std::string& log_name);

    // 增加时间戳

    std::string getCurrentTime();

private:

    std::string logPath;
    std::string logName;
};