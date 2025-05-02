#include "clientComonFunc.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include "spdlog/sinks/rotating_file_sink.h"
#include <memory>
#include <filesystem>


ClientComonFunc::ClientComonFunc()
{
    logName = "../logs/client/logfile";
    logPath = "../logs/client";
    init_logger(logName);
}

ClientComonFunc::~ClientComonFunc()
{
}

ClientComonFunc &ClientComonFunc::getInstance()
{
    static ClientComonFunc instance;
    return instance;
}

void ClientComonFunc::init_logger(const std::string &log_name)
{
            // 自动创建日志目录
    std::filesystem::create_directories(logPath);

    // 控制台彩色输出
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug);

    // 文件输出（支持每天一个文件）
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        log_name, 1024 * 1024 * 5, 3);
    file_sink->set_level(spdlog::level::debug);;

    // 日志器合并
    std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};
    auto logger = std::make_shared<spdlog::logger>("main_logger", sinks.begin(), sinks.end());

    // 设置为默认 logger
    spdlog::set_default_logger(logger);
    spdlog::flush_on(spdlog::level::info); // 自动 flush

    // 日志等级与格式
    spdlog::set_level(spdlog::level::debug); // 显示全部
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%#] %v");
}
