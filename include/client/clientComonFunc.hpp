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
    
    void init_logger(const std::string& log_name);

private:

    std::string logPath;
    std::string logName;
};