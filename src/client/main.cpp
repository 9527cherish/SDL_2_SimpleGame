#include "clientComonFunc.hpp"
#include "characterStruct.hpp"
#include "assetRepository.hpp"
#include "interfaceManager.hpp"
#include "gameInterface.hpp"
#include "Network/netClient.hpp"

// 客户端程序入口：初始化公共资源并启动界面主循环。
int main() {  // int argc, char* argv[]
    ClientComonFunc::installCrashHandlers();

    ClientComonFunc::getInstance();
    AssetRepository::getInstance().init();
    
    try {
        InterfaceManager::getInstance().start();
    } catch (const std::exception& ex) {
        ClientComonFunc::showCrashDialog("崩溃", ex.what());
        return 1;
    } catch (...) {
        ClientComonFunc::showCrashDialog("崩溃", "程序发生了未知异常。");
        return 1;
    }

    NetClient::getInstance().leaveGame();
    
    return 0;
}
