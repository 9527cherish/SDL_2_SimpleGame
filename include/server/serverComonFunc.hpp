#pragma once

#include <string>

class ServerComonFunc
{

public:
    ServerComonFunc();
    ~ServerComonFunc();

    static ServerComonFunc& getInstance();
    
    void init_logger(const std::string& log_name);

private:

    std::string logPath;
    std::string logName;
};