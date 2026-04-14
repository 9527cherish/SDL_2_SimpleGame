#include "clientComonFunc.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include "spdlog/sinks/rotating_file_sink.h"
#include <memory>
#include <filesystem>
#include <ctime>
#include <SDL2/SDL.h>
#include <csignal>
#include <exception>

namespace {
void signalHandler(int signum) {
    switch (signum) {
        case SIGSEGV:
            ClientComonFunc::showCrashDialog("崩溃", "程序发生了内存访问错误 (SIGSEGV)。");
            break;
        case SIGABRT:
            ClientComonFunc::showCrashDialog("崩溃", "程序异常终止 (SIGABRT)。");
            break;
        case SIGFPE:
            ClientComonFunc::showCrashDialog("崩溃", "程序发生了算术错误 (SIGFPE)。");
            break;
        case SIGILL:
            ClientComonFunc::showCrashDialog("崩溃", "程序发生了非法指令 (SIGILL)。");
            break;
        case SIGBUS:
            ClientComonFunc::showCrashDialog("崩溃", "程序发生了总线错误 (SIGBUS)。");
            break;
        default:
            ClientComonFunc::showCrashDialog("崩溃", "程序发生了未知错误。");
            break;
    }
    std::_Exit(1);
}

void terminateHandler() {
    try {
        std::rethrow_exception(std::current_exception());
    } catch (const std::exception& ex) {
        ClientComonFunc::showCrashDialog("崩溃", ex.what());
    } catch (...) {
        ClientComonFunc::showCrashDialog("崩溃", "程序发生了未知异常。");
    }
    std::_Exit(1);
}
}

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

std::string ClientComonFunc::getCurrentTime()
{
    std::time_t now = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&now));
    std::string time = std::string(buf);
    return time;
}

void ClientComonFunc::showCrashDialog(const char* title, const char* message)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, nullptr);
}

void ClientComonFunc::installCrashHandlers()
{
    spdlog::info("安装崩溃处理器");
    std::set_terminate(terminateHandler);
    std::signal(SIGSEGV, signalHandler);
    std::signal(SIGABRT, signalHandler);
    std::signal(SIGFPE, signalHandler);
    std::signal(SIGILL, signalHandler);
    std::signal(SIGBUS, signalHandler);
}
